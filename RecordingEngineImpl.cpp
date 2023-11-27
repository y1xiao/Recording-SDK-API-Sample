//
//  RecordingEngineImpl.cpp
//  recording sdk
//
//  Created by HN on 2023/8/8.
//

#include "RecordingEngineImpl.h"
#include "IAgoraService.h"
#include "NGIAgoraLocalUser.h"
#include "NGIAgoraAudioTrack.h"
#include "NGIAgoraMediaNodeFactory.h"
#include "NGIAgoraMediaNode.h"
#include "NGIAgoraVideoTrack.h"
#include "NGIAgoraMediaMuxer.h"
namespace agora {
namespace recording {

#if defined(TARGET_OS_LINUX)
#define DEFAULT_LOG_PATH ("./io.agora.rtc_sdk/agorasdk.log")
#else
#define DEFAULT_LOG_PATH ("/data/local/tmp/agorasdk.log")
#endif

#define DEFAULT_LOG_SIZE (512 * 1024)  // default log size is 512 kb


IRecordingEngine* IRecordingEngine::createAgoraRecordingEngine(const char * appId, IRecordingEngineEventHandler *eventHandler) {

    if(NULL == appId || !strcmp(appId,"")){
        AG_LOG(ERROR,"app id is empty");
        return NULL;
    }

    if(NULL == eventHandler){
        AG_LOG(ERROR,"event handler is empty");
        return NULL;
    }

    return new RecordingEngineImpl(appId, eventHandler);
}

RecordingEngineImpl::RecordingEngineImpl(const char * appId, IRecordingEngineEventHandler *eventHandler){
    m_appid = appId;
    m_eventHandler = eventHandler;
}

RecordingEngineImpl::~RecordingEngineImpl() {
    
}

// @WARNING : IAgoraService is Global singleton ！！Just create it once and make sure you don't destroy it before process end.
// @WARNING : Be careful when you Mult_thread programming！！
agora::base::IAgoraService* RecordingEngineImpl::createAndInitAgoraService(bool enableAudioDevice,
                                                      bool enableAudioProcessor, bool enableVideo,bool enableuseStringUid,bool enablelowDelay,const char* appid) {
  int32_t buildNum = 0;
  getAgoraSdkVersion(&buildNum);
#if defined(SDK_BUILD_NUM)
  if ( buildNum != SDK_BUILD_NUM ) {
    AG_LOG(ERROR, "SDK VERSION CHECK FAILED!\nSDK version: %d\nAPI Version: %d\n", buildNum, SDK_BUILD_NUM);
    //return nullptr;
  }
#endif
  AG_LOG(INFO, "SDK version: %d\n", buildNum);
  auto service = createAgoraService();
  agora::base::AgoraServiceConfiguration scfg;
  scfg.appId = appid;
  scfg.enableAudioProcessor = enableAudioProcessor;
  scfg.enableAudioDevice = enableAudioDevice;
  scfg.enableVideo = enableVideo;
  scfg.useStringUid = enableuseStringUid;
  if(enablelowDelay){
    scfg.channelProfile = agora::CHANNEL_PROFILE_TYPE::CHANNEL_PROFILE_CLOUD_GAMING;
  }
  if (service->initialize(scfg) != agora::ERR_OK) {
    return nullptr;
  }

  AG_LOG(INFO, "Created log file at %s", DEFAULT_LOG_PATH);
  if (service->setLogFile(DEFAULT_LOG_PATH, DEFAULT_LOG_SIZE) != 0) {
    return nullptr;
  }
// if(verifyLicense() != 0) return nullptr;
  return service;
}

int RecordingEngineImpl::joinChannel(const char * channelKey, const char *channelId, uid_t uid, const RecordingConfig &config){
    // Create Agora service
    m_service = createAndInitAgoraService(false, true, true);
    if (!m_service) {
        AG_LOG(ERROR, "Failed to creating Agora service!");
    }

    // Create Agora connection
    agora::rtc::RtcConnectionConfiguration ccfg;
    ccfg.clientRoleType = agora::rtc::CLIENT_ROLE_AUDIENCE;
    ccfg.autoSubscribeAudio = config.autoSubscribe;
    ccfg.autoSubscribeVideo = config.autoSubscribe;
    ccfg.enableAudioRecordingOrPlayout = false; // Subscribe audio but without playback

    subscribeConnection = m_service->createRtcConnection(ccfg);
    if (!subscribeConnection) {
        AG_LOG(ERROR, "Failed to creating Agora connection!");
        return -1;
    }
    // Subcribe streams from all remote users or specific remote user
    agora::rtc::VideoSubscriptionOptions subscriptionOptions;
    if (config.streamType == REMOTE_VIDEO_STREAM_HIGH) {
        subscriptionOptions.type = agora::rtc::VIDEO_STREAM_HIGH;
    } else if (config.streamType == REMOTE_VIDEO_STREAM_LOW) {
        subscriptionOptions.type = agora::rtc::VIDEO_STREAM_LOW;
    } else {
        AG_LOG(ERROR, "It is a error stream type");
        return -1;
    }
    subscribeConnection->getLocalUser()->subscribeAllAudio();
    subscribeConnection->getLocalUser()->subscribeAllVideo(subscriptionOptions);

    agora::agora_refptr<agora::rtc::IMediaNodeFactory> factory = m_service->createMediaNodeFactory();
    if (!factory) {
        AG_LOG(ERROR, "Failed to create media node factory!");
    }
    
    m_videoMixer = factory->createVideoMixer();
    if (!m_videoMixer) {
        AG_LOG(ERROR, "Failed to create video frame sender!");
        return -1;
    }
    
    // Register connection observer to monitor connection event
    m_subConnObserver = std::make_shared<SubscriberConnectionObserver>();
    subscribeConnection->registerObserver(m_subConnObserver.get());

    // Create local user observer
    m_subUserObserver = std::make_shared<SubscriberLocalUserObserver>(subscribeConnection->getLocalUser());

    m_mixVideoTrack =
    m_service->createMixedVideoTrack(m_videoMixer);
    if (!m_mixVideoTrack) {
        AG_LOG(ERROR, "Failed to create video track!");
        return -1;
    }

    m_mixVideoTrack->setVideoEncoderConfiguration(config.mixResolution);
    
    if (subscribeConnection->connect(channelKey, channelId,
                            std::to_string(uid).c_str())) {
        AG_LOG(ERROR, "Failed to connect to Agora channel!");
        return -1;
    }
    
    return 0;
}

int RecordingEngineImpl::leaveChannel(){
    return 0;
}

int RecordingEngineImpl::release(){
    return 0;
}

int RecordingEngineImpl::startService(const char * channelKey, const char *channelId, uid_t uid){
    agora::rtc::RtcConnectionConfiguration ccfg;
    ccfg.clientRoleType = agora::rtc::CLIENT_ROLE_BROADCASTER;
    ccfg.autoSubscribeAudio = false;
    ccfg.autoSubscribeVideo = false;
    ccfg.enableAudioRecordingOrPlayout = false; // Subscribe audio but without playback

    mixedConnection = m_service->createRtcConnection(ccfg);
    if (!mixedConnection) {
        AG_LOG(ERROR, "Failed to creating Agora connection!");
        return -1;
    }
    
    if (!m_mixVideoTrack) {
        AG_LOG(ERROR, "Failed to create video track!");
        return -1;
    }
    
    m_mixVideoTrack->setEnabled(true);
    
    agora::agora_refptr<agora::rtc::IMediaNodeFactory> factory = m_service->createMediaNodeFactory();
    
    auto AParameter = mixedConnection->getAgoraParameter();
    
    AParameter->setBool("rtc.video.mute_me", true);
    
    mixedConnection->getLocalUser()->publishVideo(m_mixVideoTrack);
    
    m_videoMuxer = factory->createMediaMuxer();
    
    m_encodedFrameObserver = std::make_shared<EncodedFrameObserver>(m_videoMuxer);
    
    mixedConnection->getLocalUser()->registerLocalVideoEncodedFrameObserver(m_encodedFrameObserver.get());
    
    
    // Connect to Agora channel
    if (mixedConnection->connect(channelKey, channelId,
                                 std::to_string(uid).c_str())) {
        AG_LOG(ERROR, "Failed to connect to Agora channel!");
        return -1;
    }
    m_videoMixer->setBackground(640, 480, 15, "/Users/hn/Logs/1.png");
    // add a Image to video mixer
    {
        agora::rtc::MixerLayoutConfig mixConfig;
        mixConfig.y = -10;
        mixConfig.x = -20;
        mixConfig.height = 640;
        mixConfig.width = 480;
        mixConfig.zOrder = 30;
        mixConfig.image_path = "/Users/hn/Logs/1.png";
        //mixConfig.image_path = "/data/build/hn/agora_rtc_sdk/example/video_mixer/1.jpg";
        int r = m_videoMixer->addImageSource("LocalPng", mixConfig, agora::rtc::kPng);
        //videoMixer->setStreamLayout("1", mixConfig);

        m_videoMixer->refresh();
    }
    
    m_mediaRecorderObserver = std::make_shared<MediaRecorderObserver>();
    m_videoMuxer->setMediaRecorderObserver(m_mediaRecorderObserver.get());
    
    agora::media::MediaRecorderConfiguration config;
    config.storagePath = "/Users/hn/Logs/1.mp4";
    //config.storagePath = "/data/build/hn/agora_rtc_sdk/example/video_mixer/1.mp4";
    config.streamType = agora::media::STREAM_TYPE_VIDEO;
    
    m_videoMuxer->startRecording(config);
    
    mixedConnection->getLocalUser()->forceNextIntraFrame();
    
    return 0;
}

int RecordingEngineImpl::stopService(){
    return 0;
}

}
}
