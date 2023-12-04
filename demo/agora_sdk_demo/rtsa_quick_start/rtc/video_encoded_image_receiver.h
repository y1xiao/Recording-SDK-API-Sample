//
//  Agora Media SDK
//
//  Copyright (c) 2018 Agora IO. All rights reserved.
//

#pragma once

#include "NGIAgoraMediaNodeFactory.h"
#include "utils/common_utils.h"

struct EncodedVideoFrameResult {
  agora::rtc::EncodedVideoFrameInfo frameInfo;
};

class AsyncVideoEncodedFrameObserver : public agora::media::IVideoEncodedFrameObserver {
 public:
  AsyncVideoEncodedFrameObserver() = default;
  virtual ~AsyncVideoEncodedFrameObserver() = default;

  AsyncResult<EncodedVideoFrameResult>& AsyncEncodedVideoFrameResult() {
    return encoded_video_frame_result_;
  }

  int getCount() { return receive_count_; }

 private:
  bool onEncodedVideoFrameReceived(
      agora::rtc::uid_t uid, const uint8_t* imageBuffer, size_t length,
      const agora::rtc::EncodedVideoFrameInfo& encodedVideoFrameInfo) override;

  AsyncResult<EncodedVideoFrameResult> encoded_video_frame_result_;
  int receive_count_ = 0;
};