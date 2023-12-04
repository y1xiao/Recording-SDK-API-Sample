//  Agora RTC/MEDIA SDK
//
//  Created by Pengfei Han in 2020-03.
//  Copyright (c) 2020 Agora.io. All rights reserved.
//

#pragma once

#include "IAgoraService.h"
#include "NGIAgoraRtcConnection.h"
#include "AgoraBase.h"
#include "local_user_wrapper.h"
#include "utils/common_utils.h"

#include <map>

struct ConnectionConfig {
  bool play = false;
  int minPort = 0;
  int maxPort = 0;
  size_t numberOfChannels_ = 2;
  uint32_t sampleRateHz_ = 48000;
  agora::rtc::CLIENT_ROLE_TYPE clientRoleType = agora::rtc::CLIENT_ROLE_BROADCASTER;
  bool subscribeAllAudio = false;
  bool subscribeAllVideo = false;
  agora::rtc::VIDEO_STREAM_TYPE type = agora::rtc::VIDEO_STREAM_HIGH;
  bool encodedFrameOnly = false;
  agora::CHANNEL_PROFILE_TYPE channelProfile = agora::CHANNEL_PROFILE_LIVE_BROADCASTING;
  bool audio_recv_media_packet = false;
  bool video_recv_media_packet = false;
};

static const int DefaultConnectWaitTime = 3000;

class ConnectionWrapper : public agora::rtc::IRtcConnectionObserver {
 public:
  ConnectionWrapper();
  virtual ~ConnectionWrapper();

  bool Connect(const char* appid, const char* channelId, agora::user_id_t userId,
               int waitMs = DefaultConnectWaitTime);
  bool Disconnect(int waitMs = DefaultConnectWaitTime);
  std::shared_ptr<LocalUserWrapper> GetLocalUser();

  int CreateDataStream(int& streamId, bool reliable, bool ordered);
  int SendStreamMessage(int streamId, const char* data, size_t length);
  int sendIntraRequest(agora::user_id_t userId);

  agora::rtc::TConnectionInfo getConnectionInfo();

  bool getDataStreamStats(agora::user_id_t userId, int streamId, DataStreamResult& result);

  void clearDataStreamStats() { data_stream_stats_.clear(); }

 public:
  // inherit from agora::rtc::IRtcConnectionObserver
  void onConnected(const agora::rtc::TConnectionInfo& connectionInfo,
                   agora::rtc::CONNECTION_CHANGED_REASON_TYPE reason) override;

  void onDisconnected(const agora::rtc::TConnectionInfo& connectionInfo,
                      agora::rtc::CONNECTION_CHANGED_REASON_TYPE reason) override;

  void onConnecting(const agora::rtc::TConnectionInfo& connectionInfo,
                    agora::rtc::CONNECTION_CHANGED_REASON_TYPE reason) override {}

  void onReconnecting(const agora::rtc::TConnectionInfo& connectionInfo,
                      agora::rtc::CONNECTION_CHANGED_REASON_TYPE reason) override {}

  void onReconnected(const agora::rtc::TConnectionInfo& connectionInfo,
                     agora::rtc::CONNECTION_CHANGED_REASON_TYPE reason) override {}

  void onConnectionLost(const agora::rtc::TConnectionInfo& connectionInfo) override {}

  void onLastmileQuality(const agora::rtc::QUALITY_TYPE quality) override {}

  void onTokenPrivilegeWillExpire(const char* token) override {}

  void onTokenPrivilegeDidExpire() override {}

  void onConnectionFailure(const agora::rtc::TConnectionInfo& connectionInfo,
                           agora::rtc::CONNECTION_CHANGED_REASON_TYPE reason) override {}

  void onUserJoined(agora::user_id_t userId) override {}

  void onUserLeft(agora::user_id_t userId, agora::rtc::USER_OFFLINE_REASON_TYPE reason) override {}

  void onTransportStats(const agora::rtc::RtcStats& stats) override {}

  void onLastmileProbeResult(const agora::rtc::LastmileProbeResult& result) override {}

  void onChannelMediaRelayStateChanged(int state, int code) override {}

  void onStreamMessageError(agora::user_id_t userId, int streamId, int code, int missed,
                            int cached) override;
  void onContentInspectResult(media::CONTENT_INSPECT_RESULT result) override { return; }
  void onSnapshotTaken(const char* channel, uid_t uid, const char* filePath, int width, int height,
                       int errCode) override {
    return;
  }
};
