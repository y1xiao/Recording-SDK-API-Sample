#!/bin/bash

target="rtsa"
build_op="build"
script="toolchains/x86_64.cmake"
arch_abi="x86_64"

clean_build_files() {
    rm -rf build_linux
}

cmake_build() {
    mkdir -p build_linux
    cd build_linux
    cmake \
    -DCMAKE_BUILD_TYPE=Debug \
    -DTARGET_OS=linux \
    -DANDROID_ABI=${arch_abi} \
    -DTARGET=${target} \
    -DCMAKE_TOOLCHAIN_FILE=${script} \
    ..
    cd -
}

build() {
    cd build_linux
    make -j4
}

install_demo_dependency() {
    if [ ! -d "third_party/lib" ]; then
        ./sync-libs.sh
    fi
}

while getopts :t:b:s:a: opt
do
    case $opt in
        t) target=${OPTARG}
        ;;
        b) build_op=${OPTARG}
        ;;
        s) script=${OPTARG}
        ;;
	    a) arch_abi=${OPTARG}
	    ;;
        \?)
        echo -e "$0 [-t <target>] [-b <build_op>] [-s <script>] [-a <arch_abi>]"
        echo -e "\t -t <target>, rtsa|rtc|audio, default: rtsa"
        echo -e "\t -b <build_option>, build|clean|rebuild, default: build"
        echo -e "\t -s <Path/Of/Script>, script file for toolchain, default:toolchains/x86_64.cmake"
        echo -e "\t -a <arch_abi>, x86|x86_64|armeabi-v7a|arm64-v8a, default:x86_64"
        exit 1;
    esac
done

echo -e "\033[0;32m --> target: ${target}, arch_abi: ${arch_abi}, build_op: ${build_op}, script:${script}\033[0;0m"

if [ ${target} != "rtsa" -a ${target} != "rtc" ]; then
    echo "error target: ${target}"
    exit 1
fi

if [ ${build_op} != "build" -a ${build_op} != "rebuild"  -a ${build_op} != "clean" ]; then
    echo "error build_op: ${build_op}"
    exit 1
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
    if [ ! -d "build_linux" ]; then
        cmake_build
    fi
    build
fi

exit 0
