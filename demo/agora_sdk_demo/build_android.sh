#!/bin/bash

target="rtsa"
arch_abi="arm64-v8a"
abi_version=24
build_op="build"
sanitizer="false"

install_demo_dependency() {
    if [ ! -d "third_party/lib" ]; then
        ./sync-libs.sh
    fi
}

packetizer_libs() {
    if [ ! -d "./bin/${arch_abi}/libs/" ]; then
        mkdir -p ./bin/${arch_abi}/libs/
    fi

    if [ ${target} == "rtsa" ]; then
        lib_sdk=libagora-rtsa-sdk.so
    else
        lib_sdk=libagora-rtc-sdk.so
    fi

    cp ../../../sdk/${arch_abi}/${lib_sdk} ./bin/${arch_abi}/libs/
    cp ./bin/${arch_abi}/libAgoraSDKWrapper.so ./bin/${arch_abi}/libs/
    cp -r ../test_data ./bin/${arch_abi}/
}

clean_build_files() {
    rm -rf build_android
}

cmake_build() {
    mkdir -p build_android
    cd build_android

    cmake -DCMAKE_BUILD_TYPE=Debug \
        -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK}/build/cmake/android.toolchain.cmake \
        -DANDROID_NDK=$ANDROID_NDK \
        -DANDROID_ABI=${arch_abi} \
        -DANDROID_NATIVE_API_LEVEL=${abi_version} \
        -DANDROID_STL=c++_static \
        -DTARGET=${target} \
        -DTARGET_OS=android \
        -DMEM_CHECK=${sanitizer} \
        ..
    cd -
}

build() {
    cd build_android
    make -j4
    # archive all .so together for convenience
    packetizer_libs
}

while getopts :t:a:v:b:m opt
do
    case $opt in
        t) target=${OPTARG}
        ;;
        a) arch_abi=${OPTARG}
        ;;
        v) abi_version=${OPTARG}
        ;;
        b) build_op=${OPTARG}
        ;;
        m) sanitizer="true"
        ;;
        \?)
        echo -e "$0 [-t <target>] [-b <build_op>] [-v <abi_version>] [-a <arch_abi>] [-m]"
        echo -e "\t -t <target>, rtsa|rtc, default: rtsa"
        echo -e "\t -b <build_option>, build|clean|rebuild, default: build"
        echo -e "\t -v <abi_version>, version of android ndk, default:24"
        echo -e "\t -a <arch_abi>, x64|armeabi-v7a|arm64-v8a, default:arm64-v8a"
        echo -e "\t -m open sanitizer"
        exit 1;
    esac
done
echo -e "\033[0;32m --> target: ${target}, arch_abi: ${arch_abi}, abi_version: ${abi_version}, build_op: ${build_op}, sanitizer: ${sanitizer}\033[0;0m"

if [ ${target} != "rtsa" -a ${target} != "rtc" ]; then
    echo "error target: ${target}"
    exit 1
fi

if [ ${target} == "rtc" ]; then
    echo "android rtc is not supported for this demo"
    exit 1
fi

if [ ${arch_abi} != "armeabi-v7a" -a ${arch_abi} != "arm64-v8a" -a ${arch_abi} != "x86" -a ${arch_abi} != "x86_64" ]; then
    echo "error arch_abi: ${arch_abi}"
    exit 1
fi

if [ ${build_op} != "build" -a ${build_op} != "rebuild"  -a ${build_op} != "clean" ]; then
    echo "error build_op: ${build_op}"
    exit 1
fi

if [ ${build_op} == "clean" ]; then
    rm -rf build_android
    exit 0
fi


# download third_party lib if needed.
install_demo_dependency

if [ ${build_op} == "clean" ]; then
    clean_build_files
    exit 0
fi

if [ ${build_op} == "rebuild" ]; then
    clean_build_files
    cmake_build
    build
    exit 0
fi

if [ ${build_op} == "build" ]; then
    if [ ! -d "build_android/bin/${arch_abi}" ]; then
        cmake_build
    fi
    build
fi

exit 0
