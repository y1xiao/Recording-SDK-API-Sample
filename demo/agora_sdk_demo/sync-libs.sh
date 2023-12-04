export AGORA_SDK_DEMO_PATH=.

export HEADER_PATH=$AGORA_SDK_DEMO_PATH/third_party/include

export LIB_PATH_ARM=$AGORA_SDK_DEMO_PATH/third_party/lib/armeabi-v7a
export LIB_PATH_ARM64=$AGORA_SDK_DEMO_PATH/third_party/lib/arm64-v8a
export LIB_PATH_X64=$AGORA_SDK_DEMO_PATH/third_party/lib/x86_64
export LIB_PATH_X86=$AGORA_SDK_DEMO_PATH/third_party/lib/x86

echo sync-rtsa: mkdir -p $HEADER_PATH $LIB_PATH_ARM $LIB_PATH_ARM64 $LIB_PATH_X64 $LIB_PATH_X86
mkdir -p $HEADER_PATH $LIB_PATH_ARM $LIB_PATH_ARM64 $LIB_PATH_X64 $LIB_PATH_X86

echo sync-rtsa: get files
headers='--header=X-JFrog-Art-Api:'$JFROG_TOKEN
wget $headers https://artifactory.agoralab.co/artifactory/SDK_repo/test_data/foreman_frames.h -P $HEADER_PATH/
wget $headers https://artifactory.agoralab.co/artifactory/SDK_repo/opusfile_static/1.0.0/linux/arm/libopusfile_static.a -P $LIB_PATH_ARM/
wget $headers https://artifactory.agoralab.co/artifactory/SDK_repo/opusfile_static/1.0.0/linux/arm64/libopusfile_static.a -P $LIB_PATH_ARM64/
wget $headers https://artifactory.agoralab.co/artifactory/SDK_repo/opusfile_static/1.0.0/linux/x64/libopusfile_static.a -P $LIB_PATH_X64/
