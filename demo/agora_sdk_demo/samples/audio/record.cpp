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
#include "NGIAgoraAudioTrack.h"
#include "NGIAgoraLocalUser.h"
#include "NGIAgoraMediaNode.h"
#include "NGIAgoraMediaNodeFactory.h"
#include "NGIAgoraRtcConnection.h"
#include "NGIAgoraVideoTrack.h"
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

static void sampleSendAudioFrame(
    agora::agora_refptr<agora::rtc::IAudioPcmDataSender> audioPcmDataSender) {
  static FILE* pfile = NULL;
  int sampleSize = sizeof(int16_t) * g_options.audio.numOfChannels;
  int samplesPer10ms = g_options.audio.sampleRate / 100;
  // byte size for 10ms audio samples
  size_t sendBytes = sampleSize * samplesPer10ms;

  if (!pfile) {
    if (!(pfile = fopen(g_options.audioFile.c_str(), "r"))) {
      AG_LOG(ERROR, "Open %s failed!\n", g_options.audioFile.c_str());
      return;
    } else {
      AG_LOG(INFO, "Begin of file %s...\n", g_options.audioFile.c_str());
    }
  }

  if (feof(pfile)) {
    AG_LOG(INFO, "End of file...\n");
    fclose(pfile);
    pfile = NULL;
    return;
  }

  unsigned char frameBuffer[sendBytes];
  memset(frameBuffer, 0, sizeof(frameBuffer));

  size_t n = fread(frameBuffer, 1, sizeof(frameBuffer), pfile);
  audioPcmDataSender->sendAudioPcmData(frameBuffer, 0, 0, samplesPer10ms,
                                       agora::rtc::TWO_BYTES_PER_SAMPLE,
                                       g_options.audio.numOfChannels, g_options.audio.sampleRate);
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
  optParser.add_long_opt("userId", &g_options.userId, "User Id / default is empty");
  optParser.add_long_opt("audioFile", &g_options.audioFile,
                         "Output audio file / default is " DEFAULT_AUDIO_DEVICE);
  optParser.add_long_opt("sampleRate", &g_options.audio.sampleRate,
                         "Sample rate for received audio / default is " STR(DEFAULT_SAMPLE_RATE));
  optParser.add_long_opt(
      "numOfChannels", &g_options.audio.numOfChannels,
      "Number of channels for received audio / default is " STR(DEFAULT_NUM_OF_CHANNELS));
  optParser.add_long_opt("volume", &g_options.volume,
                         "audio volume for record / default is " STR(DEFAULT_VOLUME));

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
  scfg.enableAudioDevice = false;
  scfg.enableVideo = false;

  auto result = service->initialize(scfg);
  if (result != agora::ERR_OK) {
    AG_LOG(ERROR, "Agora service initialize failed!:result:%d\n", result);
    return -1;
  }

  // Create send connection and join channel
  agora::rtc::RtcConnectionConfiguration ccfg;
  ccfg.minPort = 0;
  ccfg.maxPort = 0;
  ccfg.clientRoleType = agora::rtc::CLIENT_ROLE_BROADCASTER;
  ccfg.channelProfile = agora::CHANNEL_PROFILE_LIVE_BROADCASTING;
  ccfg.audioRecvMediaPacket = false;
  ccfg.autoSubscribeAudio = false;
  ccfg.autoSubscribeVideo = false;

  conn = service->createRtcConnection(ccfg);

  conn->getLocalUser()->setUserRole(agora::rtc::CLIENT_ROLE_BROADCASTER);
  // Connect to Agora channel
  if (conn->connect(g_options.appId.c_str(), g_options.channelId.c_str(),
                    g_options.userId.c_str())) {
    AG_LOG(ERROR, "Connect to Agora channel failed!\n");
    return -1;
  }
  // Create media node factory
  auto factory = service->createMediaNodeFactory();
  if (!factory) {
    AG_LOG(ERROR, "Failed to create media node factory!\n");
    return -1;
  }
  // Create audio data sender
  auto audioPcmDataSender = factory->createAudioPcmDataSender();
  if (!audioPcmDataSender) {
    AG_LOG(ERROR, "Failed to create audio data sender!\n");
    return -1;
  }
  // Create audio track
  auto audio_track = service->createCustomAudioTrack(audioPcmDataSender);
  if (!audio_track) {
    AG_LOG(ERROR, "Failed to create audio track!\n");
    return -1;
  }
  // Get and ajust publish volume
  int curVolume = -1;
  if (audio_track->getPublishVolume(&curVolume)) {
    AG_LOG(ERROR, "Get current publish volume error\n");
  }
  AG_LOG(INFO, "Current publish volume:%d\n", curVolume);
  if (g_options.volume != DEFAULT_VOLUME && audio_track->adjustPublishVolume(g_options.volume)) {
    AG_LOG(ERROR, "Adjust publish volume error\n");
  }
  // Publish audio track
  audio_track->enableLocalPlayback(false);
  audio_track->setEnabled(true);
  conn->getLocalUser()->publishAudio(audio_track);

  while (!g_bSignalStop) {
    sampleSendAudioFrame(audioPcmDataSender);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  return 0;
}
