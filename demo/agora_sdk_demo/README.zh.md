
## 概述


**Agora SDK Samples** 位于samples目录，是 **Agora RTC SDK** 的基本示例代码。其中包含了对各种音频、视频格式的媒体流的发送和接收。每个示例代码可以单独运行。
提供这一系列示例代码的目的，是供开发者体验和学习声网的**RTC SDK API**，并且在理解API的基础上，更轻松的集成将功能到自己的应用程序中。

## Agora SDK Samples 编译 (For Linux)

```
$ ./build_linux.sh
$ ./sync-data.sh

# build_linux.sh 参数说明
# -t 参数为 rtsa / rtc ，选择该demo是基于RTSA SDK还是RTC SDK（Server SDK Demo应选择rtc）
# -b 参数为 build / rebuild / clean ，默认为 build
# -s 参数为 <Path/Of/Script>，cmake工具链脚本路径，由用户修改。默认为toolchains/x86_64.cmake
# -a 参数为 armeabi-v7a / arm64-v8a / x86 / x86_64 选择架构，默认为 x86_64

编译完成后，在build_linux/bin/x64目录里会产生一系列 **sample_xxx** 可执行文件。
另外，执行sync-data.sh的作用是把实例代码中依赖的一些测试媒体文件同步到本地的test_data目录。
```


## Agora SDK Samples 编译 (For Android)

```
$ ./build_android.sh

# build_android.sh 参数说明
# -a 参数为 armeabi-v7a / arm64-v8a / x86 / x86_64 选择架构，默认为 arm64-v8a
# -v 参数为 Android API Level ，默认为 24
# -b 参数为 build / rebuild / clean ，默认为 rebuild
# -m 开启 sanitizer ，默认不开启

编译完成后，在build_android/bin/${arch_abi}目录里会产生一系列 **sample_xxx** 可执行文件。
为了在Android设备上运行这些程序，需要将该目录(包括里面的libs目录)整体push到Android设备上某个有执行权限的目录，比如/data 目录，并且在运行sample前指定动态库的搜索路径
$ export LD_LIBRARY_PATH=./libs
```

## Agora SDK Samples 运行

#### 参数

**sample_send_yuv_pcm** 示例程序用来展示 **YUV** 和 **PCM** 推流的相关功能，支持的参数选项如下：

* **--token ：** 用于指定用户的appId或token。无默认值，必填
* **--channelId ：** 用于指定加入频道的名称。无默认值，必填
* **--userId ：** 用于指定用户ID。默认值为 **0**，代表SDK随机指定
* **--audioFile ：** 用于指定发送的 **PCM** 音频文件。参数为文件路径，默认值为 **test_data/test.pcm**
* **--videoFile ：** 用于指定发送的 **YUV** 视频文件。参数为文件路径，默认值为 **test_data/stefan_cif.yuv**
* **--sampleRate ：** 用于指定发送的音频的采样率。默认值为 **48000**
* **--numOfChannels ：** 用于指定发送的音频的通道数。默认值为 **1**（单通道）
* **--fps ：** 用于指定发送的视频的帧率。默认值为 **15**
* **--width ：** 用于指定发送的视频的像素宽度。默认值为 **352**
* **--height ：** 用于指定发送的视频的像素高度。默认值为 **288**
* **--bitrate ：** 用于指定发送的视频的码率。默认值为 **1000000**

**sample_send_h264_pcm** 示例程序用来展示 **H264** 和 **PCM** 推流的相关功能，支持的参数选项如下：

* **--token ：** 用于指定用户的appId或token。无默认值，必填
* **--channelId ：** 用于指定加入频道的名称。无默认值，必填
* **--userId ：** 用于指定用户ID。默认值为 **0**，代表SDK随机指定
* **--audioFile ：** 用于指定发送的 **PCM** 音频文件。参数为文件路径，默认值为 **test_data/test.pcm**
* **--videoFile ：** 用于指定发送的 **H264** 视频文件。参数为文件路径，默认值为 **test_data/test_multi_slice.h264**
* **--sampleRate ：** 用于指定发送的音频的采样率。默认值为 **48000**
* **--numOfChannels ：** 用于指定发送的音频的通道数。默认值为 **1**（单通道）
* **--fps ：** 用于指定发送的视频的帧率。默认值为 **30**

**sample_send_h264_opus** 示例程序用来展示 **H264** 和 **Opus** 推流的相关功能，支持的参数选项如下：

* **--token ：** 用于指定用户的appId或token。无默认值，必填
* **--channelId ：** 用于指定加入频道的名称。无默认值，必填
* **--userId ：** 用于指定用户ID。默认值为 **0**，代表SDK随机指定
* **--audioFile ：** 用于指定发送的 **Opus** 音频文件。参数为文件路径，默认值为 **test_data/ehren-paper_lights-96.opus**
* **--videoFile ：** 用于指定发送的 **H264** 视频文件。参数为文件路径，默认值为 **test_data/test_multi_slice.h264**
* **--fps ：** 用于指定发送的视频的帧率。默认值为 **30**

**sample_receive_yuv_pcm** 示例程序用来展示 **YUV** 和 **PCM** 拉流的相关功能，支持的参数选项如下：

* **--token ：** 用于指定用户的appId或token。无默认值，必填
* **--channelId ：** 用于指定加入频道的名称。无默认值，必填
* **--userId ：** 用于指定用户ID。默认值为 **0**，代表SDK随机指定
* **--remoteUserId ：** 用于指定远端用户ID，意思是仅订阅该远端用户发送的媒体流。无默认值，如不指定该参数，表示订阅所有远端用户发送的媒体流。
* **--audioFile ：** 用于指定音频文件名，该文件保存接收到的 **PCM** 音频流。参数为文件路径，默认值为 **received_audio.pcm**
* **--videoFile ：** 用于指定视频文件名，该文件保存接收到的 **YUV** 视频流。参数为文件路径，默认值为 **received_video.yuv**
* **--sampleRate ：** 用于指定接收的音频的采样率。默认值为 **48000**
* **--numOfChannels ：** 用于指定接收的音频的通道数。默认值为 **1**（单通道）
* **--streamtype ：** 用于指定接收的视频的大小流。默认值为**high**（大流），小流为**low**

**sample_receive_h264_pcm** 示例程序用来展示 **H264** 和 **PCM** 拉流的相关功能，支持的参数选项如下：

* **--token ：** 用于指定用户的appId或token。无默认值，必填
* **--channelId ：** 用于指定加入频道的名称。无默认值，必填
* **--userId ：** 用于指定用户ID。默认值为 **0**，代表SDK随机指定
* **--remoteUserId ：** 用于指定远端用户ID，意思是仅订阅该远端用户发送的媒体流。无默认值，如不指定该参数，表示订阅所有远端用户发送的媒体流。
* **--audioFile ：** 用于指定音频文件名，该文件保存接收到的 **PCM** 音频流。参数为文件路径，默认值为 **received_audio.pcm**
* **--videoFile ：** 用于指定视频文件名，该文件保存接收到的 **YUV** 视频流。参数为文件路径，默认值为 **received_video.h264**
* **--sampleRate ：** 用于指定接收的音频的采样率。默认值为 **48000**
* **--numOfChannels ：** 用于指定接收的音频的通道数。默认值为 **1**（单通道）
* **--streamtype ：** 用于指定接收的视频的大小流。默认值为**high**（大流），小流为**low**

**sample_send_h264_multi_inst** 示例程序用来展示单进程多实例 **H264** 推流的相关功能，支持的参数选项如下：

* **--token ：** 用于指定用户的appId或token。无默认值，必填
* **--channelId ：** 用于指定加入频道的名称。无默认值，必填
* **--userId ：** 用于指定用户ID。默认值为 **0**，代表SDK随机指定
* **--videoFile ：** 用于指定发送的 **H264** 视频文件。参数为文件路径，默认值为 **test_data/test_multi_slice.h264**
* **--fps ：** 用于指定发送的视频的帧率。默认值为 **30**
* **--numOfInstances ：** 用于指定并行发送视频流的实例数量（线程数）。默认值为 **1**

**sample_receive_decrypted_h264** 示例程序用来展示（加密） **decryted_H264** 拉流的相关功能，支持的参数选项如下：

* **--token ：** 用于指定用户的appId或token。无默认值，必填
* **--channelId ：** 用于指定加入频道的名称。无默认值，必填
* **--userId ：** 用于指定用户ID。默认值为 **0**，代表SDK随机指定
* **--remoteUserId ：** 用于指定远端用户ID，意思是仅订阅该远端用户发送的媒体流。无默认值，如不指定该参数，表示订阅所有远端用户发送的媒体流。
* **--videoFile ：** 用于指定视频文件名，该文件保存接收到的 **H264** 视频流。参数为文件路径，默认值为 **received_video.h264**
* **--encrypt ：** 用于开启或关闭加密功能。默认值为**false**（关闭）。如需开启设置为**1**
* **--encryptionKey ：** 用于设置加密的密钥。可以为任意字符串。注意：如果开启加密功能却未设置密钥，程序会报错退出

**sample_send_encrypted_h264** 示例程序用来展示（加密） **decryted_H264** 推流的相关功能，支持的参数选项如下：

* **--token ：** 用于指定用户的appId或token。无默认值，必填
* **--channelId ：** 用于指定加入频道的名称。无默认值，必填
* **--userId ：** 用于指定用户ID。默认值为 **0**，代表SDK随机指定
* **--remoteUserId ：** 用于指定远端用户ID，意思是仅订阅该远端用户发送的媒体流。无默认值，如不指定该参数，表示订阅所有远端用户发送的媒体流。
* **--videoFile ：** 用于指定发送的 **H264** 视频文件。参数为文件路径，默认值为 **test_data/test_multi_slice.h264**
* **--encrypt ：** 用于开启或关闭加密功能。默认值为**false**（关闭）。如需开启设置为**1**
* **--encryptionKey ：** 用于设置加密的密钥。可以为任意字符串。注意：如果开启加密功能却未设置密钥，程序会报错退出

注意::
如果解密失败（如，某一端加密模块未开启或者双方的密钥不一致），则接收端无法接收到h264文件。

**sample_send_pcm_by_cloudProxy** 示例程序用来展示云代理 **Cloud_Proxy**的相关功能，支持的参数选项如下：

* **--token ：** 用于指定用户的appId或token。无默认值，必填
* **--channelId ：** 用于指定加入频道的名称。无默认值，必填
* **--userId ：** 用于指定用户ID。默认值为 **0**，代表SDK随机指定
* **--audioFile ：** 用于指定发送的 **PCM** 音频文件。参数为文件路径，默认值为 **test_data/test.pcm**
* **--sampleRate ：** 用于指定接收的音频的采样率。默认值为 **48000**
* **--numOfChannels ：** 用于指定接收的音频的通道数。默认值为 **1**（单通道）
* **--cloudProxy ：** 用于打开或关闭云代理功能。默认值为**false**（关闭）如需开启设置为**1**

注：云代理的相关代码和设置脚本，在samples/cloud_proxy文件夹下。启动脚本set_proxy.sh,设置防火墙（注意此处的防火墙仅仅是关闭了一些声网正常工作所需的端口），并打开特定的白名单。脚本运行后，只有开启云代理功能后，才能正常推流，不开启云代理功能则不能正常推流。启动脚本reset.sh将恢复正常的防火墙设置（打开相应端口）。


**sample_stringuid_send** **sample_stringuid_receive** 示例程序用来展示stringuid功能，支持的参数选项和**sample_send/receive_h264_pcm** 保持一致，需要注意的如下：

* 需要保证加入频道的所有用户的stringUid保持不同，否则SDK会产生预期外的行为
* 需要频道内所有用户均开启stringuid功能，否则开启stringUid与未开启stringUid的用户无法互通


#### 例子

```
// 推流
# 加入名为`test_cname`的频道，用默认参数发送YUV视频和PCM音频，参数`XXXXXX`需要替换为用户自己的appId或token
$ build_linux/bin/x64/sample_send_yuv_pcm --token XXXXXX --channelId test_cname

# 加入名为`test_cname`的频道，用默认参数发送H264视频和PCM音频，参数`XXXXXX`需要替换为用户自己的appId或token
$ build_linux/bin/x64/sample_send_h264_pcm --token XXXXXX --channelId test_cname

# 加入名为`test_cname`的频道，用默认参数发送H264视频和Opus音频，参数`XXXXXX`需要替换为用户自己的appId或token
$ build_linux/bin/x64/sample_send_h264_opus --token XXXXXX --channelId test_cname

# 以用户ID`123456`加入名为`test_cname`的频道，发送YUV视频和PCM音频，视频分辨率为`640x360`，视频目标码率为`500kbps`，参数`XXXXXX`需要替换为用户自己的appId或token
$ build_linux/bin/x64/sample_send_yuv_pcm --token XXXXXX --channelId test_cname --userId 123456 --audioFile test_audio.pcm --videoFile test_video.yuv --width 640 --height 360 --bitrate 500000

# 加入名为`test_cname`的频道，用默认参数，分8路并行发送H264视频，参数`XXXXXX`需要替换为用户自己的appId或token
$ build_linux/bin/x64/sample_send_h264_multi_inst --token XXXXXX --channelId test_cname --numOfInstances 8

// 拉流
# 加入名为`test_cname`的频道，用默认参数接收YUV视频和PCM音频，参数`XXXXXX`需要替换为用户自己的appId或token
$ build_linux/bin/x64/sample_receive_yuv_pcm --token XXXXXX --channelId test_cname

# 加入名为`test_cname`的频道，用默认参数接收H264视频和PCM音频，参数`XXXXXX`需要替换为用户自己的appId或token
$ build_linux/bin/x64/sample_receive_yuv_pcm --token XXXXXX --channelId test_cname

# 加入名为`test_cname`的频道，订阅并接收来自于用户ID为`123456`的YUV视频和PCM音频，接收保存的音频采样率为`48000`，音频通道数为`2`，参数`XXXXXX`需要替换为用户自己的appId或token
$ build_linux/bin/x64/sample_receive_yuv_pcm --token XXXXXX --channelId test_cname --remoteUserId 123456 --sampleRate 48000 --numOfChannels 2 --audioFile test_audio.pcm --videoFile test_video.yuv
```

## Agora SDK Samples（audio）运行

#### 参数

**sample_audio运行** 示例程序用来展示 **PCM** 推流和拉流以及音频播放的相关功能，支持的参数选项如下：

* **--token ：** 用于指定用户的appId或token。无默认值，必填
* **--channelId ：** 用于指定加入频道的名称。默认值，agora_audio_test
* **--userId ：** 用于指定用户ID。默认值为 **0**，代表SDK随机指定
* **--subscribeId ：** 用于指定对端用户ID。默认值为 **0**，代表订阅播放该channelId所有用户
* **--audioFile ：** 用于指定发送的 **PCM** 音频文件。参数为文件路径，默认值为 **test_data/test.pcm**
* **--sampleRate ：** 用于指定发送的音频的采样率。默认值为 **48000**
* **--numOfChannels ：** 用于指定发送的音频的通道数。默认值为 **1**（单通道）

**sample_audio_record运行** 示例程序用来展示 **PCM** 推流的相关功能，支持的参数选项如下：

* **--token ：** 用于指定用户的appId或token。无默认值，必填
* **--channelId ：** 用于指定加入频道的名称。默认值，agora_audio_test
* **--userId ：** 用于指定用户ID。默认值为 **0**，代表SDK随机指定
* **--audioFile ：** 用于指定发送的 **PCM** 音频文件。参数为文件路径，默认值为 **test_data/test.pcm**
* **--sampleRate ：** 用于指定发送的音频的采样率。默认值为 **48000**
* **--numOfChannels ：** 用于指定发送的音频的通道数。默认值为 **1**（单通道）
* **--volume ：** 用于调节推流音量。默认值为 **-1** (不调节音量)

**sample_audio_playback运行** 示例程序用来展示 **PCM** 拉流以及音频播放的相关功能，支持的参数选项如下：

* **--token ：** 用于指定用户的appId或token。无默认值，必填
* **--channelId ：** 用于指定加入频道的名称。默认值，agora_audio_test
* **--subscribeId ：** 用于指定对端用户ID。默认值为 **0**，代表订阅播放该channelId所有用户
* **--sampleRate ：** 用于指定发送的音频的采样率。默认值为 **48000**
* **--numOfChannels ：** 用于指定发送的音频的通道数。默认值为 **1**（单通道）
* **--volume ：** 用于调节拉流播放音量。默认值为 **-1** (不调节音量)

**sample_send_h264_dual_stream** 示例程序用来展示 **H264** 推大小流的相关功能，支持的参数选项如下：

* **--token ：** 用于指定用户的appId或token。无默认值，必填
* **--channelId ：** 用于指定加入频道的名称。无默认值，必填
* **--userId ：** 用于指定用户ID。默认值为 **0**，代表SDK随机指定
* **--HighVideoFile ：** 用于指定发送的大流 **H264** 视频文件。参数为文件路径，默认值为 **test_data/test_multi_slice.h264**
* **--LowVideoFile ：** 用于指定发送的小流 **H264** 视频文件。参数为文件路径，默认值为 **test_data/test_multi_slice.h264** 默认与大流为同一文件
* **--fps ：** 用于指定发送的视频的帧率。默认值为 **30**

#### 例子

```
// 推流
# 用默认参数发送文件test_data/test.pcm音频，频道名为`test_cname`，参数`XXXXXX`需要替换为用户自己的appId或token
$ build_linux/bin/armeabi-v7a/sample_audio_record --token XXXXXX

# 采集发送小爱音箱/tmp/mico-voip-audio.fifo的PCM音频，频道名为`test_cname`，用户ID为`123456`，音量为50， 音频采样率为16000，通道数为1，参数`XXXXXX`需要替换为用户自己的appId或token
$ build_linux/bin/armeabi-v7a/sample_audio_record --token XXXXXX --channelId test_cname --userId 123456 --audioFile /tmp/mico-voip-audio.fifo --volume 50 --sampleRate 16000 --numOfChannels 1

// 拉流及播放
# 用默认参数接收播放PCM音频，频道名为`test_cname`，参数`XXXXXX`需要替换为用户自己的appId或token
$ build_linux/bin/armeabi-v7a/sample_audio_playback --token XXXXXX --channelId test_cname

# 接收指定用户ID为`123456`的PCM音频，频道名为`test_cname`，音量为50，音频采样率为`48000`，音频通道数为`1`，参数`XXXXXX`需要替换为用户自己的appId或token
$ build_linux/bin/armeabi-v7a/sample_audio_playback --token XXXXXX --channelId test_cname --subscribeId 123456 --volume 50 --sampleRate 48000 --numOfChannels 1

//语音通话（采集、推流、拉流及播放）
# 采集发送小爱音箱/tmp/mico-voip-audio.fifo的PCM音频，频道名为`test_cname`，用户ID为`123`，对端用户ID为`456`，音频采样率为16000，通道数为1，参数`XXXXXX`需要替换为用户自己的appId或token
$ build_linux/bin/armeabi-v7a/sample_audio --token XXXXXX --channelId test_cname --userId 123 --subscribeId 456 --audioFile /tmp/mico-voip-audio.fifo --sampleRate 16000 --numOfChannels 1

```
