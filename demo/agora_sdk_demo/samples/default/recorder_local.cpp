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
#include "common/muxer_manager.h"

#include "NGIAgoraAudioTrack.h"
#include "NGIAgoraLocalUser.h"
#include "NGIAgoraMediaNodeFactory.h"
#include "NGIAgoraMediaNode.h"
#include "NGIAgoraVideoTrack.h"
#include "NGIAgoraMediaMuxer.h"
#include "NGIAgoraVideoMixerSource.h"

#define DEFAULT_VIDEO_FILE "received_video.yuv"
#define DEFAULT_FILE_LIMIT (100 * 1024 * 1024)
#define STREAM_TYPE_HIGH "high"
#define STREAM_TYPE_LOW "low"

struct SampleOptions {
    std::string appId;
    std::string token;
    std::string channelId;
    std::string userId;
    std::string remoteUserId;
    std::string recordingDir;
    std::string streamType = STREAM_TYPE_HIGH;
    std::string videoFile = DEFAULT_VIDEO_FILE;
    int idle = 0;
    bool useStringUid = false;
};

class MediaRecorderObserver : public agora::media::IMediaRecorderObserverEx {
 public:
    MediaRecorderObserver() {}

    void onRecorderStateChanged(agora::media::RecorderState state, agora::media::RecorderReasonCode error) override{
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

bool onceExitFlag = false;
void SignalHandler(int sigNo)
{
    onceExitFlag = true;
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
    return true;
}

int main(int argc, char *argv[])
{
    SampleOptions options;
    opt_parser optParser;

    optParser.add_long_opt("appid", &options.appId, "The token for authentication / must");
    optParser.add_long_opt("channelId", &options.channelId, "Channel Id / must");
    optParser.add_long_opt("token", &options.token, "Channel Id / must");
    optParser.add_long_opt("userId", &options.userId, "User Id / default is 0");
    optParser.add_long_opt("Recording_Dir", &options.recordingDir,
                           "The remote user to receive stream from");
    optParser.add_long_opt("useStringUid", &options.useStringUid, "UseStringUid / default false");
    optParser.add_long_opt("streamtype", &options.streamType, "the stream type");
    optParser.add_long_opt("idle", &options.idle, "the idle time");

    if ((argc <= 1) || !optParser.parse_opts(argc, argv)) {
        std::ostringstream strStream;
        optParser.print_usage(argv[0], strStream);
        std::cout << strStream.str() << std::endl;
    }

    if (options.appId.empty()) {
        AG_LOG(ERROR, "Must provide appId!");
       return -1;
    }

    if (options.userId.empty()) {
        AG_LOG(ERROR, "Must provide userId!");
       return -1;
    }
    
    if (options.channelId.empty()) {
        AG_LOG(ERROR, "Must provide channelId!");
        return -1;
    }

    std::signal(SIGQUIT, SignalHandler);
    std::signal(SIGABRT, SignalHandler);
    std::signal(SIGINT, SignalHandler);

    // Create Agora service
    auto service = createAndInitAgoraService(false, true, true, options.useStringUid, options.appId.c_str());
    
    
    if (!service) {
        AG_LOG(ERROR, "Failed to creating Agora service!");
        return -1;
    }

    // Create Agora connection
    agora::rtc::RtcConnectionConfiguration ccfg;
    ccfg.clientRoleType = agora::rtc::CLIENT_ROLE_AUDIENCE;
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
    if (options.remoteUserId.empty()) {
        AG_LOG(INFO, "Subscribe streams from all remote users");
        connection->getLocalUser()->subscribeAllAudio();
        connection->getLocalUser()->subscribeAllVideo(subscriptionOptions);
    }
    
    agora::agora_refptr<agora::rtc::IMediaNodeFactory> factory = service->createMediaNodeFactory();
    if (!factory) {
        AG_LOG(ERROR, "Failed to create media node factory!");
    }
    
    MuxerManager* muxerManager = new MuxerManager(connection.get(), options.recordingDir, factory.get());
    
    // Register connection observer to monitor connection event
    auto connObserver = std::make_shared<SampleConnectionObserver>(options.idle ? options.idle : 300);
    connection->registerObserver(connObserver.get());

    // Create local user observer
    auto localUserObserver = std::make_shared<SampleLocalUserObserver>(connection->getLocalUser());
    
    agora::agora_refptr<agora::rtc::IMediaMuxer> videoMuxer = factory->createMediaMuxer();

    auto AParameter = connection->getAgoraParameter();

    AParameter->setBool("rtc.api_treat_string_uid_as_int_uid", true);
    
    connection->getLocalUser()->registerVideoEncodedFrameObserver(muxerManager);
    connection->getLocalUser()->setPlaybackAudioFrameBeforeMixingParameters(1, 48000);
    connection->getLocalUser()->registerAudioFrameObserver(muxerManager);
    // Connect to Agora channel
    if (connection->connect(options.token.c_str(), options.channelId.c_str(),
                            options.userId.c_str())) {
        AG_LOG(ERROR, "Failed to connect to Agora channel!");
        return -1;
    }

    MediaRecorderObserver* mediaRecorderObserver = new MediaRecorderObserver();
    videoMuxer->setMediaRecorderObserver(mediaRecorderObserver);
    
    agora::media::MediaRecorderConfiguration config;
    config.storagePath = "/Users/hn/Logs/1.mp4";
    config.streamType = agora::media::STREAM_TYPE_VIDEO;
    
    //videoMuxer->startRecording(config);
    
    // Start receiving incoming media data
    AG_LOG(INFO, "Start receiving audio & video data ...");

    // Periodically check exit flag
    while (!onceExitFlag) {
        usleep(3000000);
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
    connection = nullptr;

    // Destroy Agora Service
    service->release();
    service = nullptr;

    return 0;
}
