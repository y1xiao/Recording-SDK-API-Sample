#! /bin/bash

# USAGE:
# build_demo.sh  <source_dir>  <sdklib_dir>  <output_dir>
# RETURN:
# true, false
# EXAMPLE:
# build_demo.sh  ~/RTE/media_sdk_script  ~/build/linux/arm/  /tmp/ && echo build demo success! || echo build demo failed!

function echo_exit()
{
    echo $1
    exit $2
}

test $# -eq 3 || echo_exit "args: $* invalid!" 1
test -d $1 || echo_exit "$1 invalid!" 1
test -d $2 || echo_exit "$2 invalid!" 1
test -d $3 || echo_exit "$3 invalid!" 1

src=$1
lib=$2
dst=$3/demo$$

dstdemo=$dst/agora_sdk_demo
dstlib=$dst/../sdk
dstinc=$dst/../sdk/low_level_api/include/

mkdir -p $dstdemo $dstlib $dstinc

srcdemo=$(find $src -regex "[^.]*/agora_sdk_demo" -type d -print -quit)
cp -a $srcdemo/* $dstdemo || echo_exit "cp $srcdemo to $dstdemo error!" 1

cd $dstdemo && ./sync-libs.sh && cd - || echo_exit "download prebuilt libs error!" 1

libfile=$(find $lib -name "libagora*.so" -print -quit)
cp $libfile $dstlib || echo_exit "cp $libfile to $dstlib error!" 1

for inc in $(find $src -regex "[^.]*interface/cpp/[^/]*" -type d)
do
	cp -a $inc/* $dstinc || echo_exit "cp $inc to $dstinc error!" 1
done

mkdir -p $dst/build \
    && cd $dst/build \
    && cmake \
        -DTARGET=rtsa \
        -DTARGET_OS=linux \
        -DCMAKE_CXX_FLAGS="-I$dst/agora_sdk_demo/third_party/include" \
        -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=address -L$dst/agora_sdk_demo/third_party/lib/x86_64/" \
        -DCMAKE_SHARED_LINKER_FLAGS="-fsanitize=address -L$dst/agora_sdk_demo/third_party/lib/x86_64/" \
        $dstdemo \
    && make -j8 \
    && echo_exit "build demo for linux successed!" 0 \
    || echo_exit "build demo for linux failed!" 1
