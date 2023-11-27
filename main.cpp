//  Agora RTC/MEDIA SDK
//
//  Created by Jay Zhang in 2020-04.
//  Copyright (c) 2020 Agora.io. All rights reserved.
//

#include <csignal>
#include <cstring>
#include <sstream>
#include <string>
#include <thread>

#include "AgoraRefCountedObject.h"
#include "IAgoraService.h"
#include "NGIAgoraRtcConnection.h"
#include "common/log.h"
#include "common/opt_parser.h"
#include "common/sample_common.h"
#include "common/sample_connection_observer.h"
#include "common/sample_local_user_observer.h"

#include "NGIAgoraAudioTrack.h"
#include "NGIAgoraLocalUser.h"
#include "NGIAgoraMediaNodeFactory.h"
#include "NGIAgoraMediaNode.h"
#include "NGIAgoraVideoTrack.h"
#include "NGIAgoraMediaMuxer.h"
#include "NGIAgoraVideoMixerSource.h"

#include "IAgoraRecordingEngine.h"

#define DEFAULT_VIDEO_FILE "received_video.yuv"
#define DEFAULT_FILE_LIMIT (100 * 1024 * 1024)
#define STREAM_TYPE_HIGH "high"
#define STREAM_TYPE_LOW "low"

struct SampleOptions {
    std::string appId;
    std::string channelId;
    std::string userId;
    std::string remoteUserId;
    std::string streamType = STREAM_TYPE_HIGH;
    std::string videoFile = DEFAULT_VIDEO_FILE;
};

class H264FrameReceiver : public agora::media::IVideoEncodedFrameObserver {
 public:
  H264FrameReceiver()
      : outputFilePath_(),
        h264File_(nullptr),
        fileCount(0),
        fileSize_(0) {}

  bool onEncodedVideoFrameReceived(agora::rtc::uid_t uid, const uint8_t* imageBuffer, size_t length,
                                   const agora::rtc::EncodedVideoFrameInfo& videoEncodedFrameInfo)  override{
    return true;
  }



 private:
  std::string outputFilePath_;
  FILE* h264File_;
  int fileCount;
  int fileSize_;
};

class MediaRecorderObserver : public agora::media::IMediaRecorderObserverEx {
 public:
    MediaRecorderObserver() {}

    void onRecorderStateChanged(agora::media::RecorderState state, agora::media::RecorderErrorCode error) override{
    return;
  }

    void onRecorderInfoUpdated(const agora::media::RecorderInfo& info) override{
      return;
    }

 private:
  std::string outputFilePath_;
  FILE* h264File_;
  int fileCount;
  int fileSize_;
};


class YuvFrameObserver : public agora::rtc::IVideoSinkBase {
public:
    YuvFrameObserver(const std::string &outputFilePath)
            : outputFilePath_(outputFilePath), yuvFile_(nullptr), fileCount(0), fileSize_(0)
    {
    }

    int onFrame(const agora::media::base::VideoFrame &videoFrame) override;

    virtual ~YuvFrameObserver() = default;

private:
    std::string outputFilePath_;
    FILE *yuvFile_;
    int fileCount;
    int fileSize_;
};

class EncodedFrameObserver : public agora::media::IVideoEncodedFrameObserver{
  public:
    EncodedFrameObserver(agora::agora_refptr<agora::rtc::IMediaMuxer>& muxer) : mediaMuxer_(muxer) {
        
        
    }
    
    bool onEncodedVideoFrameReceived(agora::rtc::uid_t uid, const uint8_t* imageBuffer, size_t length,
                                     const agora::rtc::EncodedVideoFrameInfo& videoEncodedFrameInfo){
        mediaMuxer_->pushEncodedVideo(imageBuffer, length, videoEncodedFrameInfo);
        return true;
    }
    agora::agora_refptr<agora::rtc::IMediaMuxer> mediaMuxer_;
};

int YuvFrameObserver::onFrame(const agora::media::base::VideoFrame &videoFrame)
{
    // Create new file to save received YUV frames
    // printf("it is %d %d\n",videoFrame.height,videoFrame.width);
    if (!yuvFile_) {
        std::string fileName =
                (++fileCount > 1) ? (outputFilePath_ + to_string(fileCount)) : outputFilePath_;
        if (!(yuvFile_ = fopen(fileName.c_str(), "w+"))) {
            AG_LOG(ERROR, "Failed to create received video file %s", fileName.c_str());
            return 0;
        }
        AG_LOG(INFO, "Created file %s to save received YUV frames", fileName.c_str());
    }

    // Write Y planar
    size_t writeBytes = videoFrame.yStride * videoFrame.height;
    //if (fwrite(videoFrame.yBuffer, 1, writeBytes, yuvFile_) != writeBytes) {
    //    AG_LOG(ERROR, "Error writing decoded video data: %s", std::strerror(errno));
    //    return 0;
    //}
    fileSize_ += writeBytes;

    // Write U planar
    writeBytes = videoFrame.uStride * videoFrame.height / 2;
    //if (fwrite(videoFrame.uBuffer, 1, writeBytes, yuvFile_) != writeBytes) {
    //    AG_LOG(ERROR, "Error writing decoded video data: %s", std::strerror(errno));
    //    return 0;
    //}
    fileSize_ += writeBytes;

    // Write V planar
    writeBytes = videoFrame.vStride * videoFrame.height / 2;
    //if (fwrite(videoFrame.vBuffer, 1, writeBytes, yuvFile_) != writeBytes) {
    //    AG_LOG(ERROR, "Error writing decoded video data: %s", std::strerror(errno));
    //    return 0;
    //}
    fileSize_ += writeBytes;

    // Close the file if size limit is reached
    if (fileSize_ >= DEFAULT_FILE_LIMIT) {
        fclose(yuvFile_);
        yuvFile_ = nullptr;
        fileSize_ = 0;
    }
    return 0;
};

static bool exitFlag = false;
static void SignalHandler(int sigNo)
{
    exitFlag = true;
}


class AudioFrameObserverBase: public agora::media::IAudioFrameObserverBase{
    public:
        AudioFrameObserverBase() {}
        bool onRecordAudioFrame(const char* channelId, AudioFrame& audioFrame) override {return true;}
        bool onPlaybackAudioFrame(const char* channelId, AudioFrame& audioFrame) override {return true;}
        bool onMixedAudioFrame(const char* channelId, AudioFrame& audioFrame) override;
        bool onEarMonitoringAudioFrame(AudioFrame& audioFrame) override {return true;}
        AudioParams getEarMonitoringAudioParams() override {return AudioParams();}
        int getObservedAudioFramePosition() override {return 1 << 2;}
        AudioParams getPlaybackAudioParams() override {return AudioParams();}
        AudioParams getRecordAudioParams() override {return AudioParams();}
    AudioParams getMixedAudioParams() override{return AudioParams();}
    private:
};

bool AudioFrameObserverBase::onMixedAudioFrame(const char* channelId, AudioFrame& audioFrame){
    //LK_LOG(INFO, "AudioFrameObserverBase::onMixedAudioFrame.....");
    return true;
}

int main(int argc, char *argv[])
{
    SampleOptions options;
    opt_parser optParser;

    optParser.add_long_opt("token", &options.appId, "");
    optParser.add_long_opt("channelId", &options.channelId, "apapap123");
    optParser.add_long_opt("userId", &options.userId, "User Id / default is 0");
    optParser.add_long_opt("remoteUserId", &options.remoteUserId,
                           "The remote user to receive stream from");
    optParser.add_long_opt("videoFile", &options.videoFile, "Output video file");
    optParser.add_long_opt("streamtype", &options.streamType, "the stream type");

    if ((argc <= 1) || !optParser.parse_opts(argc, argv)) {
        std::ostringstream strStream;
        optParser.print_usage(argv[0], strStream);
        std::cout << strStream.str() << std::endl;
        //return -1;
    }

    if (options.appId.empty()) {
        AG_LOG(ERROR, "Must provide appId!");
       // return -1;
    }

    if (options.channelId.empty()) {
        AG_LOG(ERROR, "Must provide channelId!");
      //  return -1;
    }

    //std::signal(SIGQUIT, SignalHandler);
    //std::signal(SIGABRT, SignalHandler);
    //std::signal(SIGINT, SignalHandler);

    // Create Agora service
    auto service = createAndInitAgoraService(false, true, true);
    if (!service) {
        AG_LOG(ERROR, "Failed to creating Agora service!");
    }

    // Create Agora connection
    agora::rtc::RtcConnectionConfiguration ccfg;
    ccfg.clientRoleType = agora::rtc::CLIENT_ROLE_BROADCASTER;
    ccfg.autoSubscribeAudio = true;
    ccfg.autoSubscribeVideo = true;
    ccfg.enableAudioRecordingOrPlayout = false; // Subscribe audio but without playback

    agora::agora_refptr<agora::rtc::IRtcConnection> connection = service->createRtcConnection(ccfg);
    if (!connection) {
        AG_LOG(ERROR, "Failed to creating Agora connection!");
        return -1;
    }

    // Subcribe streams from all remote users or specific remote user
    agora::rtc::VideoSubscriptionOptions subscriptionOptions;
    if (options.streamType == STREAM_TYPE_HIGH) {
        subscriptionOptions.type = agora::rtc::VIDEO_STREAM_HIGH;
    } else if (options.streamType == STREAM_TYPE_LOW) {
        subscriptionOptions.type = agora::rtc::VIDEO_STREAM_LOW;
    } else {
        AG_LOG(ERROR, "It is a error stream type");
        return -1;
    }
    if (options.remoteUserId.empty()) {
        AG_LOG(INFO, "Subscribe streams from all remote users");
        connection->getLocalUser()->subscribeAllAudio();
        connection->getLocalUser()->subscribeAllVideo(subscriptionOptions);

    } else {
        connection->getLocalUser()->subscribeAudio(options.remoteUserId.c_str());
        connection->getLocalUser()->subscribeVideo(options.remoteUserId.c_str(),
                                                   subscriptionOptions);
    }
    
    agora::agora_refptr<agora::rtc::IMediaNodeFactory> factory = service->createMediaNodeFactory();
    if (!factory) {
        AG_LOG(ERROR, "Failed to create media node factory!");
    }
    
    agora::agora_refptr<agora::rtc::IVideoMixerSource> videoMixer = factory->createVideoMixer();
    if (!videoMixer) {
        AG_LOG(ERROR, "Failed to create video frame sender!");
        return -1;
    }
    
    // Register connection observer to monitor connection event
    auto connObserver = std::make_shared<SampleConnectionObserver>();
    connection->registerObserver(connObserver.get());

    // Create local user observer
    auto localUserObserver = std::make_shared<SampleLocalUserObserver>(connection->getLocalUser());
    
    agora::agora_refptr<agora::rtc::ILocalVideoTrack> mixVideoTrack =
            service->createMixedVideoTrack(videoMixer);
    if (!mixVideoTrack) {
        AG_LOG(ERROR, "Failed to create video track!");
        return -1;
    }
    agora::rtc::VideoEncoderConfiguration encoderConfig;
    encoderConfig.codecType = agora::rtc::VIDEO_CODEC_H264;
    encoderConfig.dimensions.width = 1280;
    encoderConfig.dimensions.height = 720;
    encoderConfig.frameRate = 15;
    //encoderConfig.bitrate = options.video.targetBitrate + 1000;

    mixVideoTrack->setVideoEncoderConfiguration(encoderConfig);

    // // Register video frame observer to receive video stream
    agora::agora_refptr<agora::rtc::IVideoSinkBase> yuvFrameObserver =
            new agora::RefCountedObject<YuvFrameObserver>(options.videoFile);

    mixVideoTrack->addRenderer(yuvFrameObserver.get());
    agora::agora_refptr<agora::rtc::IMediaMuxer> videoMuxer = factory->createMediaMuxer();
    EncodedFrameObserver* h265FrameReceiver = new EncodedFrameObserver(videoMuxer);
    
    //printf("it is %d \n",b);
    mixVideoTrack->setEnabled(true);
    localUserObserver->setEnableVideoMix(true);

    localUserObserver->setVideoMixer(videoMixer);

    auto AParameter = connection->getAgoraParameter();
    
    AParameter->setBool("rtc.video.mute_me", true);
    
    connection->getLocalUser()->publishVideo(mixVideoTrack);
    //connection->getLocalUser()->unpublishVideo(mixVideoTrack);
    
    auto audioFrameObserver = std::make_shared<AudioFrameObserverBase>();


    connection->getLocalUser()->registerLocalVideoEncodedFrameObserver(h265FrameReceiver);
    connection->getLocalUser()->setMixedAudioFrameParameters( 1,  48000, 480);
    connection->getLocalUser()->setRecordingAudioFrameParameters( 1,  48000,agora::rtc::RAW_AUDIO_FRAME_OP_MODE_READ_ONLY, 480);
    connection->getLocalUser()->registerAudioFrameObserver(audioFrameObserver.get());
    // Connect to Agora channel
    if (connection->connect("", "apapap123",
                            "123456")) {
        AG_LOG(ERROR, "Failed to connect to Agora channel!");
        return -1;
    }


    videoMixer->setMasterClockSource();
    
    videoMixer->setBackground(1280, 720, 15);
    // add a Image to video mixer
    {
        agora::rtc::MixerLayoutConfig mixConfig;
        mixConfig.y = 0;
        mixConfig.x = 0;
        mixConfig.height = 720;
        mixConfig.width = 1280;
        mixConfig.zOrder = 0;
        mixConfig.image_path = "/Users/hn/Logs/1.png";
        int r = videoMixer->addImageSource("LocalPng", mixConfig, agora::rtc::kPng);
        //videoMixer->refresh();
        //mixConfig.image_path = "/data/build/hn/agora_rtc_sdk/example/video_mixer/1.jpg";

        //videoMixer->setStreamLayout("LocalPng", mixConfig);


    }
    
    MediaRecorderObserver* mediaRecorderObserver = new MediaRecorderObserver();
    videoMuxer->setMediaRecorderObserver(mediaRecorderObserver);
    
    agora::media::MediaRecorderConfiguration config;
    config.storagePath = "/Users/hn/Logs/1.mp4";
    //config.storagePath = "/data/build/hn/agora_rtc_sdk/example/video_mixer/1.mp4";
    config.streamType = agora::media::STREAM_TYPE_VIDEO;
    
    videoMuxer->startRecording(config);
    
    connection->getLocalUser()->forceNextIntraFrame();
    
    // Start receiving incoming media data
    AG_LOG(INFO, "Start receiving audio & video data ...");

    // Periodically check exit flag
    while (!exitFlag) {
        usleep(3000000);
        //videoMuxer->stopRecording();
    }

    // Unregister connection observer
    connection->unregisterObserver(connObserver.get());

    // Disconnect from Agora channel
    if (connection->disconnect()) {
        AG_LOG(ERROR, "Failed to disconnect from Agora channel!");
        return -1;
    }
    AG_LOG(INFO, "Disconnected from Agora channel successfully");

    // Destroy Agora connection and related resources
    localUserObserver.reset();
    yuvFrameObserver.reset();
    connection = nullptr;

    // Destroy Agora Service
    service->release();
    service = nullptr;

    return 0;
}


class RecordingEngineEventHandler : public agora::recording::IRecordingEngineEventHandler {
    
};


int main_new(int argc, char *argv[]) {
    
    RecordingEngineEventHandler* handler = new RecordingEngineEventHandler();
    
    auto engine = agora::recording::IRecordingEngine::createAgoraRecordingEngine("", handler);
    
    agora::recording::RecordingConfig config;
    //config.recordFileRootDir =
    
    engine->joinChannel("", "apapap123", 123456789, config);
    
    engine->startService("", "apapap123extra", 123456789);
    
    std::signal(SIGQUIT, SignalHandler);
    std::signal(SIGABRT, SignalHandler);
    std::signal(SIGINT, SignalHandler);
    
    while (!exitFlag) {
        usleep(3000000);
        engine->stopService();
        engine->leaveChannel();
    }
    
    return 0;
}

int main_new_2(int argc, char *argv[]) {
    //LK_LOG(INFO, "Application is starting...");
    auto service = createAgoraService();
    agora::base::AgoraServiceConfiguration scfg;
    scfg.appId = "";
    scfg.enableAudioProcessor = false;
    scfg.enableAudioDevice = false;
    scfg.enableVideo = false;
    scfg.useStringUid = true;
    scfg.audioScenario = agora::rtc::AUDIO_SCENARIO_TYPE::AUDIO_SCENARIO_MEETING;
    //TODO 确认：注解录制的频道模式必须和 Native SDK 或 Web SDK 设置的频道模式保持一致，否则可能导致问题。 enum agora::CHANNEL_PROFILE_TYPE
    scfg.channelProfile = agora::CHANNEL_PROFILE_TYPE::CHANNEL_PROFILE_LIVE_BROADCASTING;

    if (service->initialize(scfg) != agora::ERR_OK) {
       // LOG_E("SDK Engine initialize failed !!!");
        return -1;
    }
    // Create Agora connection
    agora::rtc::RtcConnectionConfiguration ccfg;
    ccfg.autoSubscribeAudio = false;
    ccfg.autoSubscribeVideo = false;
    ccfg.clientRoleType = agora::rtc::CLIENT_ROLE_AUDIENCE;
    ccfg.channelProfile = scfg.channelProfile;
    ccfg.enableAudioRecordingOrPlayout = true;
    agora::agora_refptr<agora::rtc::IRtcConnection> connection = service->createRtcConnection(ccfg);
    if (!connection) {
        //LOG_E("Failed to creating Agora connection!");
        return -1;
    }

    auto connObserver = std::make_shared<SampleConnectionObserver>();
    connection->registerObserver(connObserver.get());

    agora::rtc::VideoSubscriptionOptions subscriptionOptions;
    subscriptionOptions.encodedFrameOnly = false;
    subscriptionOptions.type = agora::rtc::VIDEO_STREAM_HIGH;
    connection->getLocalUser()->subscribeAllAudio();
    connection->getLocalUser()->subscribeAllVideo(subscriptionOptions);

    // Join the channel
    if (connection->connect("", "apapap123", "123456")) {
       // LOG_E("Failed to connect to Agora channel!");
        return -1;
    }

    // 入会之后等待录制
    while (!exitFlag) {
        usleep(3000000);
        //engine->stopService();
        //engine->leaveChannel();
    }
    
    connObserver->waitUntilConnected(5000000);

    
    
   // LK_LOG(INFO, "Waiting for joining the channel...");

    
    return 2;
}
