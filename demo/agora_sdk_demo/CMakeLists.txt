cmake_minimum_required(VERSION 3.5)
project(AgoraSDKDemo)

if("${MEM_CHECK}" STREQUAL "true")
  set(sanitizer -fsanitize=address)
endif()

if("${TARGET_OS}" STREQUAL "linux")
  add_definitions("-DTARGET_OS_LINUX")
  set(lib_sdk "agora_rtc_sdk")
  set(lib_ffmpeg "agora-ffmpeg")
  set(lib_soundtouch "agora-soundtouch")
  set(lib_thread "pthread")
  if("${TARGET}" STREQUAL "rtc")
    set(rtc_src "${CMAKE_SOURCE_DIR}/rtsa_quick_start/rtc/*")
  endif()
  set(AGORA_MEDIA_SDK_DIR ${CMAKE_SOURCE_DIR}/../../sdk)
  set(AGORA_MEDIA_SDK_HEADER_DIR
  ${CMAKE_SOURCE_DIR}/../../sdk)

  # Avoid to use features above c++11 in Linux
  set(CMAKE_CXX_STANDARD 11)
else()
  add_definitions("-DTARGET_OS_ANDROID")
  if("${TARGET}" STREQUAL "rtsa")
    set(lib_sdk "agora-rtsa-sdk")
  else()
    set(lib_sdk "agora-rtc-sdk")
  endif()
  set(lib_thread "stdc++")
  set(AGORA_MEDIA_SDK_DIR
      ${CMAKE_SOURCE_DIR}/../../sdk/${ANDROID_ABI})
  set(AGORA_MEDIA_SDK_HEADER_DIR ${CMAKE_SOURCE_DIR}/../../sdk)
  # Remove c++11 to avoid third_party libs build error
  separate_arguments(CMAKE_CXX_FLAGS)
  list(REMOVE_ITEM CMAKE_CXX_FLAGS "-std=c++11")
  foreach(item ${CMAKE_CXX_FLAGS})
    set(TEMP "${TEMP} ${item}")
  endforeach(item)
  set(CMAKE_CXX_FLAGS ${TEMP})
endif()

set(AGORA_SDK_DEMO_DIR ${CMAKE_SOURCE_DIR})

# prepare_libs(Gtest)

# Set common flags
set(CMAKE_DEBUG_POSTFIX "")
set(CMAKE_RELEASE_POSTFIX "")
set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer ${sanitizer} -Werror")

# Prepare third-party libs    ###
set(third_party_path "${CMAKE_BINARY_DIR}/../third_party/")

# include header path
include_directories("${third_party_path}/include"
                    ${AGORA_MEDIA_SDK_HEADER_DIR}/low_level_api/include)

# Link common libs
link_directories(${AGORA_MEDIA_SDK_DIR} ${third_party_path}/lib/${ANDROID_ABI})
link_libraries(${lib_sdk} ${lib_ffmpeg} ${lib_soundtouch} ${lib_thread})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/${ANDROID_ABI})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/${ANDROID_ABI})

# Build apps
add_subdirectory(samples)
#add_subdirectory(rtsa_quick_start/src)
# add_subdirectory(src/test)
