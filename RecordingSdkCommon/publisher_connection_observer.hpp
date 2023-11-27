//
//  publisher_connection_observer.hpp
//  recording sdk
//
//  Created by HN on 2023/8/14.
//

#ifndef publisher_connection_observer_hpp
#define publisher_connection_observer_hpp

#include "NGIAgoraRtcConnection.h"
#include "IAgoraRecordingEngine.h"
#include "NGIAgoraMediaMuxer.h"

namespace agora {
namespace recording {

class PublisherConnectionObserver : public agora::rtc::IRtcConnectionObserver {
public:
    PublisherConnectionObserver()
    {
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
    }
    void onLastmileProbeResult(const agora::rtc::LastmileProbeResult &result) override
    {
    }
    void onChannelMediaRelayStateChanged(int state, int code) override
    {
    }

private:
    IRecordingEngineEventHandler* handler_;
};


class EncodedFrameObserver : public agora::media::IVideoEncodedFrameObserver{
  public:
    EncodedFrameObserver(agora::agora_refptr<agora::rtc::IMediaMuxer>& muxer) : mediaMuxer_(muxer) {
        
    }
    bool onEncodedVideoFrameReceived(agora::rtc::uid_t uid, const uint8_t* imageBuffer, size_t length,
                                 const agora::rtc::EncodedVideoFrameInfo& videoEncodedFrameInfo) override;
    private:
    agora::agora_refptr<agora::rtc::IMediaMuxer> mediaMuxer_;
};

class MediaRecorderObserver : public agora::media::IMediaRecorderObserverEx {
 public:
    MediaRecorderObserver() {}

    void onRecorderStateChanged(agora::media::RecorderState state, agora::media::RecorderErrorCode error) override;

    void onRecorderInfoUpdated(const agora::media::RecorderInfo& info) override;

 private:
};


}
}

#endif /* publisher_connection_observer_hpp */
