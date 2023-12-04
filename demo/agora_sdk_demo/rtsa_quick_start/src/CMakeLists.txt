cmake_minimum_required(VERSION 3.5)
project(AgoraRTSAQuickStart)

# Config of AgoraSDKWrapper
file(GLOB_RECURSE LOCAL_WRAPPER_CPP_FILES
     "${CMAKE_SOURCE_DIR}/rtsa_quick_start/utils/*"
     "${CMAKE_SOURCE_DIR}/rtsa_quick_start/wrapper/*")

# include header path
include_directories(
  ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/rtsa_quick_start
  ${CMAKE_SOURCE_DIR}/rtsa_quick_start/include
  ${AGORA_MEDIA_SDK_HEADER_DIR}/include)

# Build AgoraSDKWrapper library
add_library(AgoraSDKWrapper SHARED ${LOCAL_WRAPPER_CPP_FILES})
target_link_libraries(AgoraSDKWrapper opusfile_static)

# Build rtsa_quick_start
#file(GLOB LOCAL_DEMOAPP_CPP_FILES "${PROJECT_SOURCE_DIR}/rtsa_quick_start.cpp"
#     "${CMAKE_SOURCE_DIR}/rtsa_quick_start/wrapper_wrapper/*")

#add_executable(rtsa_quick_start ${LOCAL_DEMOAPP_CPP_FILES})
#target_link_libraries(rtsa_quick_start AgoraSDKWrapper)

# Build rtsa_quick_start_dl
#file(GLOB LOCAL_DEMODLAPP_CPP_FILES
#     "${PROJECT_SOURCE_DIR}/rtsa_quick_start_dl.cpp")

#add_executable(rtsa_quick_start_dl ${LOCAL_DEMODLAPP_CPP_FILES})
#target_link_libraries(rtsa_quick_start_dl AgoraSDKWrapper dl)
