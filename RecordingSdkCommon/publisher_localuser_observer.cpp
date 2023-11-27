//
//  publisher_localuser_observer.cpp
//  recording sdk
//
//  Created by HN on 2023/8/14.
//
#include "publisher_localuser_observer.hpp"

PublisherLocalUserObserver::PublisherLocalUserObserver(agora::rtc::IRtcConnection *connection)
{
}

PublisherLocalUserObserver::PublisherLocalUserObserver(agora::rtc::ILocalUser *user)
{
}

PublisherLocalUserObserver::~PublisherLocalUserObserver()
{
}

void PublisherLocalUserObserver::onUserAudioTrackSubscribed(
        agora::user_id_t userId, agora::agora_refptr<agora::rtc::IRemoteAudioTrack> audioTrack)
{
}

void PublisherLocalUserObserver::onUserVideoTrackSubscribed(
        agora::user_id_t userId, agora::rtc::VideoTrackInfo trackInfo,
        agora::agora_refptr<agora::rtc::IRemoteVideoTrack> videoTrack)
{
}

void PublisherLocalUserObserver::onFirstRemoteVideoDecoded(agora::user_id_t userId, int width,
                                                        int height, int elapsed)
{
}


void PublisherLocalUserObserver::onUserVideoTrackStateChanged(
    agora::user_id_t userId,
    agora::agora_refptr<agora::rtc::IRemoteVideoTrack> videoTrack,
    agora::rtc::REMOTE_VIDEO_STATE state,
    agora::rtc::REMOTE_VIDEO_STATE_REASON reason, int elapsed) {
}

void PublisherLocalUserObserver::onUserInfoUpdated(agora::user_id_t userId,
                                                ILocalUserObserver::USER_MEDIA_INFO msg, bool val)
{
}

void PublisherLocalUserObserver::onUserAudioTrackStateChanged(
        agora::user_id_t userId, agora::agora_refptr<agora::rtc::IRemoteAudioTrack> audioTrack,
        agora::rtc::REMOTE_AUDIO_STATE state, agora::rtc::REMOTE_AUDIO_STATE_REASON reason,
        int elapsed)
{
}

void PublisherLocalUserObserver::onIntraRequestReceived()
{
}
