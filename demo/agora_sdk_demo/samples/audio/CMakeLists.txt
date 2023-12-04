cmake_minimum_required(VERSION 3.5)
project(AudioSamples)

message("CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}")
message("TARGET_OS=${TARGET_OS}")

link_directories(${AGORA_MEDIA_SDK_DIR})

# Config of utils
file(GLOB_RECURSE LOCAL_UTILS_CPP_FILES
     "${PROJECT_SOURCE_DIR}/../common/opt_parser.cpp")

# Build record ###
add_executable(sample_audio_record ${PROJECT_SOURCE_DIR}/record.cpp
                                   ${LOCAL_UTILS_CPP_FILES})

# Build playback ###
add_executable(sample_audio_playback ${PROJECT_SOURCE_DIR}/playback.cpp
                                     ${LOCAL_UTILS_CPP_FILES})

# Build audio ###
add_executable(sample_audio ${PROJECT_SOURCE_DIR}/audio.cpp
                            ${LOCAL_UTILS_CPP_FILES})
