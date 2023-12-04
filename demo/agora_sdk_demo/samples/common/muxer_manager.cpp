
#include <chrono>
#include "muxer_manager.h"
#include "log.h"
#include "NGIAgoraMediaNodeFactory.h"

MuxerManager::~MuxerManager() {
}

void MuxerManager::startRecordingIfNeeded(agora::user_id_t userId)
{
    std::lock_guard<std::mutex> _(lock_);
    auto it = media_muxers_.find(userId);
    if (it == media_muxers_.end()) {
        createMuxerAndObserver(userId);
    }
    auto observer = media_muxer_observers_.find(userId);
    if(observer != media_muxer_observers_.end() && (observer->second->getState() == agora::media::RECORDER_STATE_ERROR || observer->second->getState() == agora::media::RECORDER_STATE_STOP)){
        startRecording(userId, observer->second->getState() == agora::media::RECORDER_STATE_ERROR);
    }
}

int MuxerManager::destroyMuxerIfNeeded(agora::user_id_t userId){
    auto observer = media_muxer_observers_.begin();
    while (observer != media_muxer_observers_.end()) {
      if (observer->second->getReason() == agora::media::RECORDER_REASON_NO_STREAM) {
          auto it = media_muxers_.find(observer->first);
          if (it == media_muxers_.end()) {
            media_muxers_.erase(it);
          }
          media_muxer_observers_.erase(observer);
      }
    }
    return 0;
}

void MuxerManager::createMuxerAndObserver(agora::user_id_t userId) {
    agora::agora_refptr<agora::rtc::IMediaMuxer> videoMuxer = factory_->createMediaMuxer();
    media_muxers_[userId] = videoMuxer;
    
    MediaRecorderObserverWithUid* mediaRecorderObserver = new MediaRecorderObserverWithUid(userId);
    videoMuxer->setMediaRecorderObserver(mediaRecorderObserver);
    media_muxer_observers_[userId] = mediaRecorderObserver;
}

void MuxerManager::startRecording(agora::user_id_t userId, bool dostop) {
    agora::media::MediaRecorderConfiguration config;
    std::string storagePath = getStoragePath(userId);
    config.storagePath = storagePath.c_str();
    config.streamType = agora::media::STREAM_TYPE_BOTH;
    if(dostop) {
        media_muxers_[userId]->stopRecording();
    }
    media_muxers_[userId]->startRecording(config);
    local_user_->sendIntraRequest(userId);
}

std::string MuxerManager::getStoragePath(agora::user_id_t userId){
    std::string storagePath = recording_dir_;
    storagePath += "/uid_";
    storagePath += userId;
    storagePath += "_";
    storagePath += std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
                                 .count());
    storagePath += ".mp4";
    return storagePath;
}

bool MuxerManager::onEncodedVideoFrameReceived(agora::rtc::uid_t uid, const uint8_t* imageBuffer, size_t length,
                                               const agora::rtc::EncodedVideoFrameInfo& videoEncodedFrameInfo) {
    agora::rtc::UserInfo info;
    connection_->getUserInfoByUid(uid, &info);
    startRecordingIfNeeded(info.userAccount);
    auto it = media_muxers_.find(info.userAccount);
    if (it != media_muxers_.end()) {
        it->second->pushEncodedVideo(imageBuffer, length, videoEncodedFrameInfo);
    }
    return true;
}

bool MuxerManager::onPlaybackAudioFrameBeforeMixing(const char* channelId, agora::media::base::user_id_t userId, AudioFrame& audioFrame) {
    startRecordingIfNeeded(userId);
    auto it = media_muxers_.find(userId);
    if (it != media_muxers_.end()) {
        it->second->pushAudioFrame(&audioFrame);
    }
    return true;
}
