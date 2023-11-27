//
//  subscriber_connection_observer.cpp
//  recording sdk
//
//  Created by HN on 2023/8/9.
//

#include "subscriber_connection_observer.hpp"

#include "log.h"


namespace agora {
namespace recording {
void SubscriberConnectionObserver::onConnected(const agora::rtc::TConnectionInfo &connectionInfo,
                                           agora::rtc::CONNECTION_CHANGED_REASON_TYPE reason)
{
    AG_LOG(INFO, "onConnected: id %u, channelId %s, localUserId %s, reason %d\n", connectionInfo.id,
           connectionInfo.channelId.get()->c_str(), connectionInfo.localUserId.get()->c_str(),
           reason);

    // notify the thread which is waiting for the SDK to be connected
}

void SubscriberConnectionObserver::onDisconnected(const agora::rtc::TConnectionInfo &connectionInfo,
                                              agora::rtc::CONNECTION_CHANGED_REASON_TYPE reason)
{
    AG_LOG(INFO, "onDisconnected: id %u, channelId %s, localUserId %s, reason %d\n",
           connectionInfo.id, connectionInfo.channelId.get()->c_str(),
           connectionInfo.localUserId.get()->c_str(), reason);

    // notify the thread which is waiting for the SDK to be disconnected
}

void SubscriberConnectionObserver::onConnecting(const agora::rtc::TConnectionInfo &connectionInfo,
                                            agora::rtc::CONNECTION_CHANGED_REASON_TYPE reason)
{
    AG_LOG(INFO, "onConnecting: id %u, channelId %s, localUserId %s, reason %d\n",
           connectionInfo.id, connectionInfo.channelId.get()->c_str(),
           connectionInfo.localUserId.get()->c_str(), reason);
}

void SubscriberConnectionObserver::onReconnecting(const agora::rtc::TConnectionInfo &connectionInfo,
                                              agora::rtc::CONNECTION_CHANGED_REASON_TYPE reason)
{
    AG_LOG(INFO, "onReconnecting: id %u, channelId %s, localUserId %s, reason %d\n",
           connectionInfo.id, connectionInfo.channelId.get()->c_str(),
           connectionInfo.localUserId.get()->c_str(), reason);
}

void SubscriberConnectionObserver::onReconnected(const agora::rtc::TConnectionInfo &connectionInfo,
                                             agora::rtc::CONNECTION_CHANGED_REASON_TYPE reason)
{
    AG_LOG(INFO, "onReconnected: id %u, channelId %s, localUserId %s, reason %d\n",
           connectionInfo.id, connectionInfo.channelId.get()->c_str(),
           connectionInfo.localUserId.get()->c_str(), reason);
}

void SubscriberConnectionObserver::onConnectionLost(const agora::rtc::TConnectionInfo &connectionInfo)
{
    AG_LOG(INFO, "onConnectionLost: id %u, channelId %s, localUserId %s\n", connectionInfo.id,
           connectionInfo.channelId.get()->c_str(), connectionInfo.localUserId.get()->c_str());
}

void SubscriberConnectionObserver::onUserJoined(agora::user_id_t userId)
{
    AG_LOG(INFO, "onUserJoined: userId %s\n", userId);
}

void SubscriberConnectionObserver::onUserLeft(agora::user_id_t userId,
                                          agora::rtc::USER_OFFLINE_REASON_TYPE reason)
{
    AG_LOG(INFO, "onUserLeft: userId %s, reason %d\n", userId, reason);
}

}
}
