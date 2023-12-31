#pragma once

#include "NGIAgoraRtcConnection.h"
#include "NGIAgoraRtmpConnection.h"
#include "sample_event.h"

extern bool onceExitFlag;

class SampleConnectionObserver : public agora::rtc::IRtcConnectionObserver,
								 public agora::rtc::INetworkObserver {
public:
	SampleConnectionObserver(uint64_t idle = 300)
	{
        idle_s_ = idle;
        last_active_ts_ = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
            .count();
	}
	int waitUntilConnected(int waitMs)
	{
		return connect_ready_.Wait(waitMs);
	}

public: // IRtcConnectionObserver
	void onConnected(const agora::rtc::TConnectionInfo &connectionInfo,
					 agora::rtc::CONNECTION_CHANGED_REASON_TYPE reason) override;
	void onDisconnected(const agora::rtc::TConnectionInfo &connectionInfo,
						agora::rtc::CONNECTION_CHANGED_REASON_TYPE reason) override;
	void onConnecting(const agora::rtc::TConnectionInfo &connectionInfo,
					  agora::rtc::CONNECTION_CHANGED_REASON_TYPE reason) override;
	void onReconnecting(const agora::rtc::TConnectionInfo &connectionInfo,
						agora::rtc::CONNECTION_CHANGED_REASON_TYPE reason) override;
	void onReconnected(const agora::rtc::TConnectionInfo &connectionInfo,
					   agora::rtc::CONNECTION_CHANGED_REASON_TYPE reason) override;
	void onConnectionLost(const agora::rtc::TConnectionInfo &connectionInfo) override;
	void onLastmileQuality(const agora::rtc::QUALITY_TYPE quality) override
	{
	}
	void onTokenPrivilegeWillExpire(const char *token) override
	{
	}
	void onTokenPrivilegeDidExpire() override
	{
	}
	void onConnectionFailure(const agora::rtc::TConnectionInfo &connectionInfo,
							 agora::rtc::CONNECTION_CHANGED_REASON_TYPE reason) override
	{
	}
	void onUserJoined(agora::user_id_t userId) override;
	void onUserLeft(agora::user_id_t userId, agora::rtc::USER_OFFLINE_REASON_TYPE reason) override;
	void onTransportStats(const agora::rtc::RtcStats &stats) override
	{
        uint64_t now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
            .count();
        if(stats.userCount > 1){
            last_active_ts_ = now_ms;
        }
        if(now_ms - last_active_ts_ > idle_s_ * 1000){
            onceExitFlag = true;
        }
	}
	void onLastmileProbeResult(const agora::rtc::LastmileProbeResult &result) override
	{
	}
	void onChannelMediaRelayStateChanged(int state, int code) override
	{
	}

public: // INetworkObserver
	void onUplinkNetworkInfoUpdated(const agora::rtc::UplinkNetworkInfo &info) override;

private:
	SampleEvent connect_ready_;
	SampleEvent disconnect_ready_;
                                     
                                     
    uint64_t last_active_ts_;
    uint64_t idle_s_;
};

class RtmpConnectionObserver : public agora::rtc::IRtmpConnectionObserver {
public:
	RtmpConnectionObserver()
	{
	}
	int waitUntilConnected(int waitMs)
	{
		return connect_ready_.Wait(waitMs);
	}

public:
	void onConnected(const agora::rtc::RtmpConnectionInfo &connectionInfo) override;
	void onDisconnected(const agora::rtc::RtmpConnectionInfo &connectionInfo) override;
	void onReconnecting(const agora::rtc::RtmpConnectionInfo &connectionInfo) override;
	void onReconnected(const agora::rtc::RtmpConnectionInfo &connectionInfo) override;
	void onConnectionFailure(const agora::rtc::RtmpConnectionInfo &connectionInfo,
							 agora::rtc::RTMP_CONNECTION_ERROR errCode) override;
	void onTransferStatistics(uint64_t video_width, uint64_t video_height, uint64_t video_bitrate,
							  uint64_t audio_bitrate, uint64_t video_frame_rate,
							  uint64_t push_video_frame_cnt, uint64_t pop_video_frame_cnt) override;

private:
	SampleEvent connect_ready_;
	SampleEvent disconnect_ready_;
};
