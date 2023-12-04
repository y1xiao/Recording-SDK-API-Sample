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
#include <vector>

#include "IAgoraService.h"
#include "NGIAgoraRtcConnection.h"
#include "common/helper.h"
#include "common/log.h"
#include "common/opt_parser.h"
#include "common/sample_common.h"
#include "common/sample_connection_observer.h"
#include "common/sample_local_user_observer.h"

#define DEFAULT_CONNECT_TIMEOUT_MS (3000)
#define DEFAULT_NUM_OF_INSTANCES (1)
#define DEFAULT_SAMPLE_RATE (48000)
#define DEFAULT_NUM_OF_CHANNELS (1)
#define DEFAULT_INPUT_AUDIO_FILE "test_data/test.pcm"
#define DEFAULT_OUTPUT_AUDIO_FILE "received_audio"

struct SampleOptions {
  std::string appId;
  std::string channelId;
  std::string userId;
  std::string inputAudioFile = DEFAULT_INPUT_AUDIO_FILE;
  std::string outputAudioFile = DEFAULT_OUTPUT_AUDIO_FILE;
  int numOfInstances = DEFAULT_NUM_OF_INSTANCES;
  struct {
    int sampleRate = DEFAULT_SAMPLE_RATE;
    int numOfChannels = DEFAULT_NUM_OF_CHANNELS;
  } audio;
};

class PcmFrameObserver : public agora::media::IAudioFrameObserver {
 public:
  PcmFrameObserver(const std::string& outputFilePath)
      : outputFilePath_(outputFilePath), pcmFile_(nullptr) {}

  bool onPlaybackAudioFrame(AudioFrame& audioFrame) override { return true; };

  bool onRecordAudioFrame(AudioFrame& audioFrame) override { return true; };

  bool onMixedAudioFrame(AudioFrame& audioFrame) override { return true; };

  bool onPlaybackAudioFrameBeforeMixing(unsigned int uid, AudioFrame& audioFrame) override;

 private:
  std::string outputFilePath_;
  FILE* pcmFile_;
};

bool PcmFrameObserver::onPlaybackAudioFrameBeforeMixing(unsigned int uid, AudioFrame& audioFrame) {
  // Create new file to save received PCM samples
  if (!pcmFile_) {
    if (!(pcmFile_ = fopen((outputFilePath_).c_str(), "w"))) {
      AG_LOG(ERROR, "Failed to create received audio file %s", outputFilePath_.c_str());
      return false;
    }
    AG_LOG(INFO, "Creat file %s successfully", outputFilePath_.c_str());
  }

  // Write PCM samples
  size_t writeBytes = audioFrame.samplesPerChannel * audioFrame.channels * sizeof(int16_t);
  if (fwrite(audioFrame.buffer, 1, writeBytes, pcmFile_) != writeBytes) {
    AG_LOG(ERROR, "Error writing decoded audio data: %s", std::strerror(errno));
    return false;
  }

  return true;
}

static void sendOnePcmFrame(const SampleOptions& options, FILE*& file,
                            agora::agora_refptr<agora::rtc::IAudioPcmDataSender> audioFrameSender) {
  const char* fileName = options.inputAudioFile.c_str();

  // Calculate byte size for 10ms audio samples
  int sampleSize = sizeof(int16_t) * options.audio.numOfChannels;
  int samplesPer10ms = options.audio.sampleRate / 100;
  int sendBytes = sampleSize * samplesPer10ms;

  if (!file) {
    if (!(file = fopen(fileName, "rb"))) {
      AG_LOG(ERROR, "Failed to open audio file %s", fileName);
      return;
    }
    AG_LOG(INFO, "Open audio file %s successfully", fileName);
  }

  uint8_t frameBuf[sendBytes];

  if (fread(frameBuf, 1, sizeof(frameBuf), file) != sizeof(frameBuf)) {
    if (feof(file)) {
      fclose(file);
      file = nullptr;
      AG_LOG(INFO, "End of audio file");
    } else {
      AG_LOG(ERROR, "Error reading audio data: %s", std::strerror(errno));
    }
    return;
  }

  if (audioFrameSender->sendAudioPcmData(
          frameBuf, 0, 0, samplesPer10ms, agora::rtc::TWO_BYTES_PER_SAMPLE,
          options.audio.numOfChannels, options.audio.sampleRate) < 0) {
    AG_LOG(ERROR, "Failed to send audio frame!");
  }
}

static void SampleSendReceivePcmTask(const SampleOptions& options,
                                     agora::base::IAgoraService* service,
                                     agora::agora_refptr<agora::rtc::IMediaNodeFactory> factory,
                                     const int& connIndex, bool& exitFlag) {
  // Create Agora connection
  agora::rtc::AudioSubscriptionOptions audioSubOpt;
  audioSubOpt.packetOnly = true;

  agora::rtc::RtcConnectionConfiguration ccfg;
  ccfg.autoSubscribeAudio = true;
  ccfg.autoSubscribeVideo = false;
  ccfg.clientRoleType = agora::rtc::CLIENT_ROLE_BROADCASTER;
  ccfg.audioSubscriptionOptions = audioSubOpt;
  agora::agora_refptr<agora::rtc::IRtcConnection> connection = service->createRtcConnection(ccfg);
  if (!connection) {
    AG_LOG(ERROR, "Failed to creating Agora connection!");
    return;
  }

  // Register connection observer to monitor connection event
  auto connObserver = std::make_shared<SampleConnectionObserver>();
  connection->registerObserver(connObserver.get());

  // Create local user observer
  auto localUserObserver = std::make_shared<SampleLocalUserObserver>(connection->getLocalUser());

  // Register audio frame observer to receive audio stream
  auto pcmFrameObserver = std::make_shared<PcmFrameObserver>(options.outputAudioFile + "_" +
                                                             std::to_string(connIndex) + ".pcm");
  if (connection->getLocalUser()->setPlaybackAudioFrameBeforeMixingParameters(
          options.audio.numOfChannels, options.audio.sampleRate)) {
    AG_LOG(ERROR, "Failed to set audio frame parameters!");
    return;
  }
  localUserObserver->setAudioFrameObserver(pcmFrameObserver.get());

  // Connect to Agora channel, with channel name as test_cname0, test_cname1, ...
  if (connection->connect(options.appId.c_str(),
                          (options.channelId + std::to_string(connIndex)).c_str(),
                          options.userId.c_str())) {
    AG_LOG(ERROR, "Failed to connect to Agora channel!");
    return;
  }

  // Create audio data sender
  agora::agora_refptr<agora::rtc::IAudioPcmDataSender> audioFrameSender =
      factory->createAudioPcmDataSender();
  if (!audioFrameSender) {
    AG_LOG(ERROR, "Failed to create audio data sender!");
    return;
  }

  // Create audio track
  agora::agora_refptr<agora::rtc::ILocalAudioTrack> customAudioTrack =
      service->createCustomAudioTrack(audioFrameSender);
  if (!customAudioTrack) {
    AG_LOG(ERROR, "Failed to create audio track!");
    return;
  }

  // Publish audio track
  connection->getLocalUser()->publishAudio(customAudioTrack);

  // Wait until connected before sending media stream
  connObserver->waitUntilConnected(DEFAULT_CONNECT_TIMEOUT_MS);

  // Currently only 10 ms PCM frame is supported. So PCM frames are sent at 10 ms interval
  PacerInfo pacer = {0, 10, std::chrono::steady_clock::now()};
  FILE* inputFile = nullptr;

  while (!exitFlag) {
    sendOnePcmFrame(options, inputFile, audioFrameSender);
    waitBeforeNextSend(pacer);  // sleep for a while before sending next frame
  }

  // Unpublish video track
  connection->getLocalUser()->unpublishAudio(customAudioTrack);

  // Unregister connection observer
  connection->unregisterObserver(connObserver.get());

  // Disconnect from Agora channel
  if (connection->disconnect()) {
    AG_LOG(ERROR, "Failed to disconnect from Agora channel!");
    return;
  }
  AG_LOG(INFO, "Disconnected from Agora channel successfully");

  // Destroy Agora connection and related resources
  connObserver.reset();
  localUserObserver.reset();
  pcmFrameObserver.reset();
  audioFrameSender = nullptr;
  customAudioTrack = nullptr;
  connection = nullptr;
}

static bool exitFlag = false;
static void SignalHandler(int sigNo) { exitFlag = true; }

int main(int argc, char* argv[]) {
  SampleOptions options;
  opt_parser optParser;

  optParser.add_long_opt("token", &options.appId, "The token for authentication / must");
  optParser.add_long_opt("channelId", &options.channelId, "Channel Id / must");
  optParser.add_long_opt("userId", &options.userId, "User Id / default is 0");
  optParser.add_long_opt("numOfInstances", &options.numOfInstances,
                         "Number of instances to run in parallel");
  optParser.add_long_opt("inputAudioFile", &options.inputAudioFile,
                         "The audio file in PCM format to be sent");
  optParser.add_long_opt("outputAudioFile", &options.outputAudioFile,
                         "The audio file in PCM format to be saved");
  optParser.add_long_opt("sampleRate", &options.audio.sampleRate,
                         "Sample rate for the PCM file to be sent");
  optParser.add_long_opt("numOfChannels", &options.audio.numOfChannels,
                         "Number of channels for the PCM file to be sent");

  if ((argc <= 1) || !optParser.parse_opts(argc, argv)) {
    std::ostringstream strStream;
    optParser.print_usage(argv[0], strStream);
    std::cout << strStream.str() << std::endl;
    return -1;
  }

  if (options.appId.empty()) {
    AG_LOG(ERROR, "Must provide appId!");
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
  auto service = createAndInitAgoraService(false, true, true);
  if (!service) {
    AG_LOG(ERROR, "Failed to creating Agora service!");
  }

  // Create media node factory
  agora::agora_refptr<agora::rtc::IMediaNodeFactory> factory = service->createMediaNodeFactory();
  if (!factory) {
    AG_LOG(ERROR, "Failed to create media node factory!");
  }

  // Start sending media data
  AG_LOG(INFO, "Start sending audio data in multiple threads ...");
  std::vector<std::thread*> sysThreads;
  for (int i = 0; i < options.numOfInstances; ++i) {
    std::thread* systhread =
        new std::thread(SampleSendReceivePcmTask, options, service, factory, i, std::ref(exitFlag));
    sysThreads.push_back(systhread);
  }

  for (auto threadIt = sysThreads.begin(); threadIt != sysThreads.end(); ++threadIt) {
    (*threadIt)->join();
  }
  while (!sysThreads.empty()) {
    std::thread* systhread = sysThreads[sysThreads.size() - 1];
    sysThreads.pop_back();
    delete systhread;
  }

  // Destroy media node Factory
  factory = nullptr;

  // Destroy Agora Service
  service->release();
  service = nullptr;

  return 0;
}
