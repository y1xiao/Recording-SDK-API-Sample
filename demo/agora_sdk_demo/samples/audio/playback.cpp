//  Agora RTC/MEDIA SDK
//
//  Created by WU FEI in 2020-06.
//  Copyright (c) 2019 Agora.io. All rights reserved.
//

#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <csignal>
#include <iostream>
#include <sstream>
#include <thread>

#include "IAgoraService.h"
#include "NGIAgoraLocalUser.h"
#include "NGIAgoraMediaNodeFactory.h"
#include "NGIAgoraRtcConnection.h"
#include "common/log.h"
#include "common/opt_parser.h"

#define STR(R) #R

#define DEFAULT_APP_ID "aab8b8f5a8cd4469a63042fcfafe7063"  // Live Broadcast
#define DEFAULT_CHANNEL_ID "agora_audio_test"
#define DEFAULT_SAMPLE_RATE 48000
#define DEFAULT_NUM_OF_CHANNELS 1
#define DEFAULT_AUDIO_DEVICE "test_data/test.pcm"
#define DEFAULT_VOLUME -1

static struct SampleOptions {
  std::string appId = DEFAULT_APP_ID;
  std::string channelId = DEFAULT_CHANNEL_ID;
  std::string userId;
  std::string subUserId;
  std::string audioFile = DEFAULT_AUDIO_DEVICE;
  int volume = DEFAULT_VOLUME;

  struct {
    int sampleRate = DEFAULT_SAMPLE_RATE;
    int numOfChannels = DEFAULT_NUM_OF_CHANNELS;
  } audio;
} g_options;

static bool g_bSignalStop = false;

static void signal_handler(int signo) {
  (void)signo;
  g_bSignalStop = true;
}

int main(int argc, char* argv[]) {
  agora::agora_refptr<agora::rtc::IRtcConnection> conn;
  signal(SIGQUIT, signal_handler);
  signal(SIGABRT, signal_handler);
  signal(SIGINT, signal_handler);

  opt_parser optParser;

  optParser.add_long_opt("token", &g_options.appId,
                         "The token for authentication / default is " DEFAULT_APP_ID);
  optParser.add_long_opt("channelId", &g_options.channelId,
                         "Channel Id / default is " DEFAULT_CHANNEL_ID);
  optParser.add_long_opt("subscribeId", &g_options.subUserId,
                         "User Id for subscribed channel / default is empty");
  optParser.add_long_opt("sampleRate", &g_options.audio.sampleRate,
                         "Sample rate for received audio / default is " STR(DEFAULT_SAMPLE_RATE));
  optParser.add_long_opt(
      "numOfChannels", &g_options.audio.numOfChannels,
      "Number of channels for received audio / default is " STR(DEFAULT_NUM_OF_CHANNELS));
  optParser.add_long_opt("volume", &g_options.volume,
                         "audio volume for playback / default is " STR(DEFAULT_VOLUME));

  if (argc > 1 && !optParser.parse_opts(argc, argv)) {
    std::ostringstream strStream;
    optParser.print_usage(argv[0], strStream);
    std::cout << strStream.str() << std::endl;
    return -1;
  }

  // Create Agora service and initialize
  auto service = createAgoraService();
  agora::base::AgoraServiceConfiguration scfg;
  scfg.enableAudioProcessor = true;
  scfg.enableAudioDevice = true;
  scfg.enableVideo = false;

  auto result = service->initialize(scfg);
  if (result != agora::ERR_OK) {
    AG_LOG(ERROR, "Agora service initialize failed!:result:%d\n", result);
    return -1;
  }

  // Create send connection and join channel
  agora::rtc::AudioSubscriptionOptions audioSubOpt;
  agora::rtc::RtcConnectionConfiguration ccfg;
  audioSubOpt.bytesPerSample = agora::rtc::TWO_BYTES_PER_SAMPLE;
  audioSubOpt.numberOfChannels = g_options.audio.numOfChannels;
  audioSubOpt.sampleRateHz = g_options.audio.sampleRate;
  ccfg.minPort = 0;
  ccfg.maxPort = 0;
  ccfg.audioSubscriptionOptions = audioSubOpt;
  ccfg.enableAudioRecordingOrPlayout = true;
  ccfg.clientRoleType = agora::rtc::CLIENT_ROLE_BROADCASTER;
  ccfg.channelProfile = agora::CHANNEL_PROFILE_LIVE_BROADCASTING;
  ccfg.audioRecvMediaPacket = false;
  ccfg.autoSubscribeAudio = g_options.subUserId.empty() ? true : false;
  ccfg.autoSubscribeVideo = false;

  conn = service->createRtcConnection(ccfg);

  conn->getLocalUser()->setUserRole(agora::rtc::CLIENT_ROLE_BROADCASTER);
  if (conn->getLocalUser()->setPlaybackAudioFrameBeforeMixingParameters(
          g_options.audio.numOfChannels, g_options.audio.sampleRate) != 0) {
    AG_LOG(ERROR, "Failed to set audio frame parameters!\n");
    return -1;
  }
  // Connect to Agora channel
  if (conn->connect(g_options.appId.c_str(), g_options.channelId.c_str(),
                    g_options.userId.c_str())) {
    AG_LOG(ERROR, "Connect to Agora channel failed!\n");
    return -1;
  }
  // Get and ajust playback volume
  int curVolume = -1;
  if (conn->getLocalUser()->getPlaybackSignalVolume(&curVolume)) {
    AG_LOG(ERROR, "Get current playback volume error\n");
  }
  AG_LOG(INFO, "Current playback volume:%d\n", curVolume);
  if (g_options.volume != DEFAULT_VOLUME &&
      conn->getLocalUser()->adjustPlaybackSignalVolume(g_options.volume)) {
    AG_LOG(ERROR, "Adjust playback volume error\n");
  }
  // Subscribe audio track
  if (g_options.subUserId.empty()) {
    conn->getLocalUser()->subscribeAllAudio();
  } else {
    conn->getLocalUser()->subscribeAudio(g_options.subUserId.c_str());
  }
  while (!g_bSignalStop) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  return 0;
}
