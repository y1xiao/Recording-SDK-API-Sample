//  Agora RTC/MEDIA SDK
//
//  Created by Pengfei Han in 2020-03.
//  Copyright (c) 2020 Agora.io. All rights reserved.
//

#pragma once
#include <map>
#include <mutex>

#include "AgoraBase.h"
#include "AgoraMediaBase.h"
#include "NGIAgoraLocalUser.h"
#include "NGIAgoraMediaMuxer.h"
#include "NGIAgoraRtcConnection.h"
#include "NGIAgoraMediaNode.h"

class MediaRecorderObserverWithUid : public agora::media::IMediaRecorderObserverEx {
 public:
    MediaRecorderObserverWithUid(agora::user_id_t userId) {
        uid_ = userId;
    }

    void onRecorderStateChanged(agora::media::RecorderState state, agora::media::RecorderReasonCode error) override{
      state_ = state;
      error_ = error;
      return;
  }

    void onRecorderInfoUpdated(const agora::media::RecorderInfo& info) override{
      return;
    }
    
    agora::media::RecorderState getState(){
        return state_;
    }

    agora::media::RecorderReasonCode getReason(){
        return error_;
    }
    
 private:
  agora::user_id_t uid_;
  agora::media::RecorderState state_ = agora::media::RECORDER_STATE_STOP;
  agora::media::RecorderReasonCode error_;
};

class MuxerManager : public agora::media::IVideoEncodedFrameObserver , public agora::media::IAudioFrameObserverBase{
 public:
    MuxerManager(agora::rtc::IRtcConnection* connection, std::string recordingDir, agora::rtc::IMediaNodeFactory* factory){
        connection_ = connection;
        local_user_ = connection_->getLocalUser();
        recording_dir_ = recordingDir;
        factory_ = factory;
    }
    ~MuxerManager();
public:
    bool onRecordAudioFrame(const char* channelId, AudioFrame& audioFrame) override {return true;}
    bool onPlaybackAudioFrame(const char* channelId, AudioFrame& audioFrame) override {return true;}
    bool onMixedAudioFrame(const char* channelId, AudioFrame& audioFrame) override {return true;}
    bool onEarMonitoringAudioFrame(AudioFrame& audioFrame) override {return true;}
    bool onPlaybackAudioFrameBeforeMixing(const char* channelId, agora::media::base::user_id_t userId, AudioFrame& audioFrame) override;
    AudioParams getEarMonitoringAudioParams() override {return AudioParams();}
    int getObservedAudioFramePosition() override {return 1 << 3;}
    AudioParams getPlaybackAudioParams() override {return AudioParams();}
    AudioParams getRecordAudioParams() override {return AudioParams();}
    AudioParams getMixedAudioParams() override{return AudioParams();}
    bool onEncodedVideoFrameReceived(agora::rtc::uid_t uid, const uint8_t* imageBuffer, size_t length,
                                     const agora::rtc::EncodedVideoFrameInfo& videoEncodedFrameInfo) override;
    
 private:
    void startRecordingIfNeeded(agora::user_id_t userId);
    int destroyMuxerIfNeeded(agora::user_id_t userId);
    void createMuxerAndObserver(agora::user_id_t userId);
    void startRecording(agora::user_id_t userId, bool dostop);
    std::string getStoragePath(agora::user_id_t userId);
    agora::rtc::IRtcConnection* connection_;
    agora::rtc::ILocalUser* local_user_{nullptr};
    std::string recording_dir_;
    std::mutex lock_;
    agora::rtc::IMediaNodeFactory* factory_{nullptr};
    std::map<std::string, agora::agora_refptr<agora::rtc::IMediaMuxer>> media_muxers_;
    std::map<std::string, MediaRecorderObserverWithUid*> media_muxer_observers_;
};
