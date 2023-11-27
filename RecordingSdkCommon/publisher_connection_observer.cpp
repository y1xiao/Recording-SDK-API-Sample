//
//  publisher_connection_observer.cpp
//  recording sdk
//
//  Created by HN on 2023/8/14.
//

#include "publisher_connection_observer.hpp"

#include "log.h"


namespace agora {
namespace recording {
void PublisherConnectionObserver::onConnected(const agora::rtc::TConnectionInfo &connectionInfo,
                                           agora::rtc::CONNECTION_CHANGED_REASON_TYPE reason)
{
    AG_LOG(INFO, "onConnected: id %u, channelId %s, localUserId %s, reason %d\n", connectionInfo.id,
           connectionInfo.channelId.get()->c_str(), connectionInfo.localUserId.get()->c_str(),
           reason);

    // notify the thread which is waiting for the SDK to be connected
}

void PublisherConnectionObserver::onDisconnected(const agora::rtc::TConnectionInfo &connectionInfo,
                                              agora::rtc::CONNECTION_CHANGED_REASON_TYPE reason)
{
    AG_LOG(INFO, "onDisconnected: id %u, channelId %s, localUserId %s, reason %d\n",
           connectionInfo.id, connectionInfo.channelId.get()->c_str(),
           connectionInfo.localUserId.get()->c_str(), reason);

    // notify the thread which is waiting for the SDK to be disconnected
}

void PublisherConnectionObserver::onConnecting(const agora::rtc::TConnectionInfo &connectionInfo,
                                            agora::rtc::CONNECTION_CHANGED_REASON_TYPE reason)
{
    AG_LOG(INFO, "onConnecting: id %u, channelId %s, localUserId %s, reason %d\n",
           connectionInfo.id, connectionInfo.channelId.get()->c_str(),
           connectionInfo.localUserId.get()->c_str(), reason);
}

void PublisherConnectionObserver::onReconnecting(const agora::rtc::TConnectionInfo &connectionInfo,
                                              agora::rtc::CONNECTION_CHANGED_REASON_TYPE reason)
{
    AG_LOG(INFO, "onReconnecting: id %u, channelId %s, localUserId %s, reason %d\n",
           connectionInfo.id, connectionInfo.channelId.get()->c_str(),
           connectionInfo.localUserId.get()->c_str(), reason);
}

void PublisherConnectionObserver::onReconnected(const agora::rtc::TConnectionInfo &connectionInfo,
                                             agora::rtc::CONNECTION_CHANGED_REASON_TYPE reason)
{
    AG_LOG(INFO, "onReconnected: id %u, channelId %s, localUserId %s, reason %d\n",
           connectionInfo.id, connectionInfo.channelId.get()->c_str(),
           connectionInfo.localUserId.get()->c_str(), reason);
}

void PublisherConnectionObserver::onConnectionLost(const agora::rtc::TConnectionInfo &connectionInfo)
{
    AG_LOG(INFO, "onConnectionLost: id %u, channelId %s, localUserId %s\n", connectionInfo.id,
           connectionInfo.channelId.get()->c_str(), connectionInfo.localUserId.get()->c_str());
}

void PublisherConnectionObserver::onUserJoined(agora::user_id_t userId)
{
    AG_LOG(INFO, "onUserJoined: userId %s\n", userId);
}

void PublisherConnectionObserver::onUserLeft(agora::user_id_t userId,
                                          agora::rtc::USER_OFFLINE_REASON_TYPE reason)
{
    AG_LOG(INFO, "onUserLeft: userId %s, reason %d\n", userId, reason);
}





    
bool EncodedFrameObserver::onEncodedVideoFrameReceived(agora::rtc::uid_t uid, const uint8_t* imageBuffer, size_t length,
                                 const agora::rtc::EncodedVideoFrameInfo& videoEncodedFrameInfo){
    mediaMuxer_->pushEncodedVideo(imageBuffer, length, videoEncodedFrameInfo);
    return true;
}

void MediaRecorderObserver::onRecorderStateChanged(agora::media::RecorderState state, agora::media::RecorderErrorCode error) {
    return;
}

void MediaRecorderObserver::onRecorderInfoUpdated(const agora::media::RecorderInfo& info) {
    return;
}



}
}
