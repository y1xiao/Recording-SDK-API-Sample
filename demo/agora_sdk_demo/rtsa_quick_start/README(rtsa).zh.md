Agora RTSA Quickstart

*[English](README(rtsa).md) | 中文*

## 概述

**RTSA Quick Start** 位于rtsa_quick_start目录，主要对rtsa的功能进行了封装，支持循环并发推拉流。


## RTSA Quick Start 编译
```cpp
编译前需要填写客户自己的 app Id 或 token Id，编译方法参考 README.zh.md
// rtsa_quick_start/wrapper/utils.h
#define API_CALL_APPID "use your own app id"  // 替换成自己 app Id 或 token Id
```


## RTSA Quick Start 运行

#### 参数

**rtsa_quick_start** 支持传递多个参数选项，来控制其行为：

* **-a ：** 用于指定音频发送测试时发送的音频类型。
    * **1**: (默认值) 表示发送 **OPUS**。
    * **3**: 表示发送 **WAV**。
    * **7**: 表示发送 **AACLC**。
* **-v ：** 用于指定视频发送测试时发送的视频类型。
    * **1**: 表示发送 **VP8**。
    * **2**: (默认值) 表示发送 **H.264**。
* **-j ：** 用于指定发送测试时的并发度，即同一时刻起的并发发送音视频流的线程数。默认值为 **1**。
* **-m ：** 用于指定发送测试时发送内容。
    * **0**: 表示 **音频和视频都不发**。
    * **1**: 表示 **只发视频**。
    * **2**: (默认值) 表示 **只发音频**。
    * **3**: 表示 **音频和视频都发**。
* **-n ：** 用于指定发送测试的运行轮次。**SDK Demo** 中用于发送测试的音频测试文件或视频测试文件时长为几十秒到几分钟，这个参数用于控制发送这些测试文件的次数。默认值为 **1**。
* **-r ：** 用于指定 demo 执行接收测试，demo 默认执行发送测试。
    * **0**: (默认值) 表示 **pull模式** 接收数据。
    * **1**: 表示 **observer模式** 接收数据。
* **-b ：** 用于指定 demo 收发 DataStream 的形式。
    * **0**: (默认值) 不开启。
    * **1**: 表示 **reliable/ordered**。
    * **2**: 表示 **unreliable/unordered**。
* **-d ：** 用于指定接收测试时的持续时间，只在 **-r** 时有用。
* **-u ：** 用于指定测试 **userId** ，如果会是多个用户测试，会在该 **userId** 基础上进行变化产生其他的 **userId** 。
* **-c ：** 用于指定测试频道名，默认频道名为 **conn_default_rtsa** 。
* **-s ：** 用于 observer 形式接收时数据，将数据保存到文件，文件格式为 **wav**，recording/playback/mixed 默认文件名是 **user_pcm_audio_data.wav**，before mixed 默认文件名是 **uid + ${uid} + _user_pcm_audio_data.wav**。
    * 参数值为 **0** 表示保存 recording 数据，即 agora::media::IAudioFrameObserver::onRecordAudioFrame 对应的 audio frame（RTSA2.0不支持该模式）。
    * 参数值为 **1** 表示保存 playback 数据，即 agora::media::IAudioFrameObserver::onPlaybackAudioFrame 对应的 audio frame。
    * 参数值为 **2** 表示保存 before mixed数据，即 agora::media::IAudioFrameObserver::onPlaybackAudioFrameBeforeMixing 对应的 audio frame。
    * 参数值为 **3** 表示保存 mixed 数据，即 agora::media::IAudioFrameObserver::onMixedAudioFrame 对应的 audio frame（RTSA2.0不支持该模式）。
* **-p ：** 用于指定音视频以 **Media Packet** 与 **Control Packet** 进行 **Raw data** 的传输，且接收端只能以 **observer** 方式，即 **-p -r 1**。

#### 例子

```
$ cd build_linux/bin/x64
$ ./rtsa_quick_start -a 8 -c rtsa           # 单轮单线程发送 AAC 测试文件，频道名为`rtsa`
$ ./rtsa_quick_start -j 2 -m 1              # 并发 2 个线程发送视频
$ ./rtsa_quick_start -n 2                   # 进行两次测试
$ ./rtsa_quick_start -r 0 -d 10000          # pull 形式接收 10 秒测试数据，单位毫秒
$ ./rtsa_quick_start -j 10 -u 10000         # 并发 10 个线程发送音视频，用户 Id 分别是 10000，10001，10002... 10009
$ ./rtsa_quick_start -r 1 -j 5 -d 20000     # 5 个用户 observer 形式接收 20 秒测试数据，单位毫秒
$ ./rtsa_quick_start -r 1 -s 1              # observer 形式接收数据并保存文件，文件名为 `user_pcm_audio_data.wav`
$ ./rtsa_quick_start -b 1                   # 以 reliable/ordered 发送 DataStream 数据
```

#### media data 相关参数
* `-p`：用于指定音视频以Media Packet与Control Packet进行Raw data的传输，且接收端只能以observer方式，即 '-p -r 1'。
* `-j`：用于指定发送测试时的并发度，即同一时刻起的并发发送音视频media data的线程数。默认值为 `1`。
* `-m`：用于指定发送测试时发送内容，参数值为 `0` 表示 `音频和视频media data都不发`，参数值为 `1` 表示 `只发视频media data`，参数值为 `2` 表示 `只发音频media data`，参数值为 `3` 表示 `音频和视频media data都发`。默认值为 `2`。
* `-n`：用于指定发送测试的运行轮次。`SDK Demo` 中用于发送测试的音频或视频media data时长为几十秒到几分钟，这个参数用于控制发送这些测试数据的次数。默认值为 `1`。
* `-r`：用于指定demo执行接收测试，demo默认执行发送测试，参数为 `0` 表示pull模式接收数据，参数为 `1` 表示observer模式接收数据，media data只能以observer模式接收数据。默认值为 `0`。
* `-d`：用于指定接收测试时的持续时间，只在`-r`时有用。
* `-u`：用于指定测试userId，如果会是多个用户测试，会在该userId基础上进行变化产生其他的userId。
* `-c`：用于指定测试频道名，默认频道名为`conn_test_zzz`。

#### 例子

```
$ ./rtsa_quick_start -p -m 1 -c test_cname  # 单轮单线程发送视频media data，频道名为`test_cname`
$ ./rtsa_quick_start -p -j 2 -m 1           # 并发2个线程发送视频media data
$ ./rtsa_quick_start -p -n 2                # 进行两次测试
$ ./rtsa_quick_start -p -r 1                # observer形式接收media data
$ ./rtsa_quick_start -p -r 1 -d 10000       # observer形式接收10秒测试数据，单位毫秒
$ ./rtsa_quick_start -p -j 10 -u 10000      # 并发10个线程发送音视频，用户Id分别是10000，10001，10002... 10009
```