//
//  subscriber_localuser_observer.hpp
//  recording sdk
//
//  Created by HN on 2023/8/11.
//

#ifndef subscriber_localuser_observer_hpp
#define subscriber_localuser_observer_hpp

#include <map>
#include <mutex>

#include "AgoraBase.h"
#include "NGIAgoraLocalUser.h"
#include "NGIAgoraRtcConnection.h"

class SubscriberLocalUserObserver : public agora::rtc::ILocalUserObserver {
 public:
  SubscriberLocalUserObserver(agora::rtc::IRtcConnection* connection);
  SubscriberLocalUserObserver(agora::rtc::ILocalUser* user);
  virtual ~SubscriberLocalUserObserver();

 public:
  // inherit from agora::rtc::ILocalUserObserver
  void onAudioTrackPublishSuccess(
      agora::agora_refptr<agora::rtc::ILocalAudioTrack> audioTrack) override {}

  void onUserAudioTrackSubscribed(
      agora::user_id_t userId,
      agora::agora_refptr<agora::rtc::IRemoteAudioTrack> audioTrack) override;

  void onAudioTrackPublicationFailure(agora::agora_refptr<agora::rtc::ILocalAudioTrack> audioTrack,
                                      agora::ERROR_CODE_TYPE error) override {}

  void onUserAudioTrackStateChanged(agora::user_id_t userId,
                                    agora::agora_refptr<agora::rtc::IRemoteAudioTrack> audioTrack,
                                    agora::rtc::REMOTE_AUDIO_STATE state,
                                    agora::rtc::REMOTE_AUDIO_STATE_REASON reason,
                                    int elapsed) override;

  void onVideoTrackPublishSuccess(agora::agora_refptr<agora::rtc::ILocalVideoTrack> videoTrack) override {}

  void onVideoTrackPublicationFailure(agora::agora_refptr<agora::rtc::ILocalVideoTrack> videoTrack,
                                      agora::ERROR_CODE_TYPE error) override {}

  void onUserVideoTrackSubscribed(
      agora::user_id_t userId, agora::rtc::VideoTrackInfo trackInfo,
      agora::agora_refptr<agora::rtc::IRemoteVideoTrack> videoTrack) override;

  void onUserVideoTrackStateChanged(agora::user_id_t userId,
                                    agora::agora_refptr<agora::rtc::IRemoteVideoTrack> videoTrack,
                                    agora::rtc::REMOTE_VIDEO_STATE state,
                                    agora::rtc::REMOTE_VIDEO_STATE_REASON reason,
                                    int elapsed) ;

  void onRemoteVideoTrackStatistics(agora::agora_refptr<agora::rtc::IRemoteVideoTrack> videoTrack,
                                    const agora::rtc::RemoteVideoTrackStats& stats) override {}

  void onLocalVideoTrackStateChanged(agora::agora_refptr<agora::rtc::ILocalVideoTrack> videoTrack,
                                     agora::rtc::LOCAL_VIDEO_STREAM_STATE state,
                                     agora::rtc::LOCAL_VIDEO_STREAM_ERROR errorCode) override {}

  void onLocalVideoTrackStatistics(agora::agora_refptr<agora::rtc::ILocalVideoTrack> videoTrack,
                                   const agora::rtc::LocalVideoTrackStats& stats) override {}

  void onAudioVolumeIndication(const agora::rtc::AudioVolumeInformation* speakers,
                               unsigned int speakerNumber, int totalVolume) override {}

  void onLocalAudioTrackStatistics(const agora::rtc::LocalAudioStats& stats) override {}

  void onRemoteAudioTrackStatistics(agora::agora_refptr<agora::rtc::IRemoteAudioTrack> audioTrack,
                                    const agora::rtc::RemoteAudioTrackStats& stats) override {}
                                  
  //void onRemoteVideoStreamInfoUpdated(const agora::rtc::RemoteVideoStreamInfo& info) override {}
  

  void onUserInfoUpdated(agora::user_id_t userId, USER_MEDIA_INFO msg, bool val) override;

  void onIntraRequestReceived() override;

  void onAudioSubscribeStateChanged(const char* channel, agora::user_id_t uid,
                                    agora::rtc::STREAM_SUBSCRIBE_STATE oldState,
                                    agora::rtc::STREAM_SUBSCRIBE_STATE newState,
                                    int elapseSinceLastState) override {}

  void onVideoSubscribeStateChanged(const char* channel, agora::user_id_t uid,
                                    agora::rtc::STREAM_SUBSCRIBE_STATE oldState,
                                    agora::rtc::STREAM_SUBSCRIBE_STATE newState,
                                    int elapseSinceLastState) override {}

  void onAudioPublishStateChanged(const char* channel, agora::rtc::STREAM_PUBLISH_STATE oldState,
                                  agora::rtc::STREAM_PUBLISH_STATE newState,
                                  int elapseSinceLastState) override {}

  void onVideoPublishStateChanged(const char* channel, agora::rtc::STREAM_PUBLISH_STATE oldState,
                                  agora::rtc::STREAM_PUBLISH_STATE newState,
                                  int elapseSinceLastState) override {}
  void onFirstRemoteVideoFrameRendered(agora::user_id_t userId, int width,
                                               int height, int elapsed){}
  void onFirstRemoteVideoFrame(agora::user_id_t userId, int width, int height, int elapsed){}
  void onFirstRemoteAudioFrame(agora::user_id_t userId, int elapsed) {}
  void onFirstRemoteAudioDecoded(agora::user_id_t userId, int elapsed) {}
  void onFirstRemoteVideoDecoded(agora::user_id_t userId, int width, int height, int elapsed) override;

  // virtual void onFirstRemoteAudioDecoded(agora::user_id_t userId, int elapsed){}
  void onAudioTrackPublishStart(agora::agora_refptr<agora::rtc::ILocalAudioTrack> audioTrack) override {}

  void onAudioTrackUnpublished(agora::agora_refptr<agora::rtc::ILocalAudioTrack> audioTrack) override {}

  void onVideoTrackPublishStart(agora::agora_refptr<agora::rtc::ILocalVideoTrack> videoTrack) override {}

  void onVideoTrackUnpublished(agora::agora_refptr<agora::rtc::ILocalVideoTrack> videoTrack)override {}

  void onVideoSizeChanged(agora::user_id_t userId, int width, int height, int rotation) {}
  void onActiveSpeaker(agora::user_id_t userId){}
 private:

};
#endif /* subscriber_localuser_observer_hpp */
