//
//  subscriber_localuser_observer.cpp
//  recording sdk
//
//  Created by HN on 2023/8/11.
//

#include "subscriber_localuser_observer.hpp"

SubscriberLocalUserObserver::SubscriberLocalUserObserver(agora::rtc::IRtcConnection *connection)
{
}

SubscriberLocalUserObserver::SubscriberLocalUserObserver(agora::rtc::ILocalUser *user)
{
}

SubscriberLocalUserObserver::~SubscriberLocalUserObserver()
{
}

void SubscriberLocalUserObserver::onUserAudioTrackSubscribed(
        agora::user_id_t userId, agora::agora_refptr<agora::rtc::IRemoteAudioTrack> audioTrack)
{
}

void SubscriberLocalUserObserver::onUserVideoTrackSubscribed(
        agora::user_id_t userId, agora::rtc::VideoTrackInfo trackInfo,
        agora::agora_refptr<agora::rtc::IRemoteVideoTrack> videoTrack)
{
}

void SubscriberLocalUserObserver::onFirstRemoteVideoDecoded(agora::user_id_t userId, int width,
                                                        int height, int elapsed)
{
}


void SubscriberLocalUserObserver::onUserVideoTrackStateChanged(
    agora::user_id_t userId,
    agora::agora_refptr<agora::rtc::IRemoteVideoTrack> videoTrack,
    agora::rtc::REMOTE_VIDEO_STATE state,
    agora::rtc::REMOTE_VIDEO_STATE_REASON reason, int elapsed) {
}

void SubscriberLocalUserObserver::onUserInfoUpdated(agora::user_id_t userId,
                                                ILocalUserObserver::USER_MEDIA_INFO msg, bool val)
{
}

void SubscriberLocalUserObserver::onUserAudioTrackStateChanged(
        agora::user_id_t userId, agora::agora_refptr<agora::rtc::IRemoteAudioTrack> audioTrack,
        agora::rtc::REMOTE_AUDIO_STATE state, agora::rtc::REMOTE_AUDIO_STATE_REASON reason,
        int elapsed)
{
}

void SubscriberLocalUserObserver::onIntraRequestReceived()
{
}
