//  Agora RTC/MEDIA SDK
//
//  Created by Pengfei Han in 2019-06.
//  Copyright (c) 2019 Agora.io. All rights reserved.
//

#include "video_file_parser_factory.h"

#include <memory>

#include "h264_file_parser.h"
#include "yuv_file_parser.h"

VideoFileParserFactory& VideoFileParserFactory::Instance() {
  static VideoFileParserFactory factory;
  return factory;
}

VideoFileParserFactory::VideoFileParserFactory() {}

VideoFileParserFactory::~VideoFileParserFactory() {}

std::unique_ptr<VideoFileParser> VideoFileParserFactory::createVideoFileParser(
    ParserConfig& config) {
  std::unique_ptr<VideoFileParser> parser;
  if (config.fileType == VIDEO_FILE_TYPE::VIDEO_FILE_H264) {
    parser = std::move(createH264FileParser(config.filePath));
  } else if (config.fileType == VIDEO_FILE_TYPE::VIDEO_FILE_YUV) {
    parser = std::move(
        createYuvFileParser(config.filePath, config.width, config.height, config.pixelFormat));
  }
  return std::move(parser);
}

std::unique_ptr<VideoFileParser> VideoFileParserFactory::createH264FileParser(
    const char* filepath) {
  std::unique_ptr<VideoFileParser> parser(new H264FileParser(filepath));
  return std::move(parser);
}

std::unique_ptr<VideoFileParser> VideoFileParserFactory::createYuvFileParser(
    const char* filepath, int width, int heigth, agora::media::base::VIDEO_PIXEL_FORMAT format) {
  std::unique_ptr<YuvFileParser> parser(new YuvFileParser(filepath, width, heigth, format));
  return std::move(parser);
}