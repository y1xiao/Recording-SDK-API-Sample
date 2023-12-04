//  Agora RTC/MEDIA SDK
//
//  Created by Pengfei Han in 2020-03.
//  Copyright (c) 2020 Agora.io. All rights reserved.
//

#include "local_user_wrapper.h"

LocalUserWrapper::LocalUserWrapper(agora::rtc::ILocalUser* local_user) : local_user_(local_user) {
  local_user_->registerLocalUserObserver(this);
}

LocalUserWrapper::~LocalUserWrapper() { local_user_->unregisterLocalUserObserver(this); }

agora::rtc::ILocalUser* LocalUserWrapper::GetLocalUser() { return local_user_; }

void LocalUserWrapper::PublishAudioTrack(
    agora::agora_refptr<agora::rtc::ILocalAudioTrack> audioTrack) {
  local_user_->publishAudio(audioTrack);
}

void LocalUserWrapper::PublishVideoTrack(
    agora::agora_refptr<agora::rtc::ILocalVideoTrack> videoTrack) {
  local_user_->publishVideo(videoTrack);
}

void LocalUserWrapper::UnpublishAudioTrack(
    agora::agora_refptr<agora::rtc::ILocalAudioTrack> audioTrack) {
  local_user_->unpublishAudio(audioTrack);
}

void LocalUserWrapper::UnpublishVideoTrack(
    agora::agora_refptr<agora::rtc::ILocalVideoTrack> videoTrack) {
  local_user_->unpublishVideo(videoTrack);
}

void LocalUserWrapper::onUserAudioTrackSubscribed(
    agora::user_id_t userId, agora::agora_refptr<agora::rtc::IRemoteAudioTrack> audioTrack) {
  std::lock_guard<std::mutex> _(observer_lock_);
  remote_audio_track_ = audioTrack;
  if (remote_audio_track_ && media_packet_receiver_) {
    remote_audio_track_->registerMediaPacketReceiver(media_packet_receiver_);
  }
  if (remote_audio_track_ && audio_frame_observer_) {
    local_user_->registerAudioFrameObserver(audio_frame_observer_);
  }
}

void LocalUserWrapper::onUserVideoTrackSubscribed(
    agora::user_id_t userId, agora::rtc::VideoTrackInfo trackInfo,
    agora::agora_refptr<agora::rtc::IRemoteVideoTrack> videoTrack) {
  std::lock_guard<std::mutex> _(observer_lock_);
  remote_video_track_ = videoTrack;
  if (remote_video_track_ && video_encoded_receiver_) {
    remote_video_track_->registerVideoEncodedFrameObserver(video_encoded_receiver_);
  }
  if (remote_video_track_ && media_packet_receiver_) {
    remote_video_track_->registerMediaPacketReceiver(media_packet_receiver_);
  }
  if (remote_video_track_ && video_frame_observer_) {
    remote_video_track_->addRenderer(video_frame_observer_);
  }
}

void LocalUserWrapper::onStreamMessage(agora::user_id_t userId, int streamId, const char* data,
                                       size_t length) {
  unsigned int uid = static_cast<unsigned int>(atoll(userId));
  auto key = std::make_pair(uid, streamId);
  if (data_stream_stats_.find(key) == data_stream_stats_.end()) {
    DataStreamResult result;
    data_stream_stats_.insert(
        std::pair<std::pair<unsigned int, int>, DataStreamResult>(key, result));
  }

  if (data == nullptr || length == 0) {
    printf("*** Data stream received fail, data = %p, length = %d\n", data,
           static_cast<int>(length));
    data_stream_stats_[key].check_result = false;
    return;
  }

  for (int i = 0; i < length; i++) {
    if (data[i] != ((i % 256) - 128)) {
      printf("*** Data stream received mismatch, data = %p, length = %d\n", data,
             static_cast<int>(length));
      data_stream_stats_[key].check_result = false;
      return;
    }
  }
  data_stream_stats_[key].received_msg_count++;
  data_stream_stats_[key].received_total_bytes += length;
  printf("Data stream received: userId = %s, streamId = %d, length = %d\n", userId, streamId,
         static_cast<int>(length));
}
