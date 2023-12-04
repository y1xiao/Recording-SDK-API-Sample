cmake_minimum_required(VERSION 3.5)
project(AgoraSDKSamples)

# include header path
include_directories(${AGORA_SDK_DEMO_DIR}/samples)

# Build apps
add_subdirectory(default)
#add_subdirectory(audio)
# add_subdirectory(cloud_gaming)
add_subdirectory(cloud_proxy)
# add_subdirectory(license)
