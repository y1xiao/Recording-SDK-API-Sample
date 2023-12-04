Agora RTSA Quickstart

*English | [中文](README(rtsa).zh.md)*

## Overview

**Agora RTSA Quickstart** is a demo used for demonstrating the basic functions of **Agora RTSA SDK**, which mainly include sending and receiving audio and video.

## Prerequisites
### Development Environment



## Compile Quickstart

```cpp
You need to replace the real app id or token in the below code. And then compile according to README.zh.md
// src/wrapper/utils.h
#define API_CALL_APPID "use your own app id" // Replace with your own appId
```


## Run RTSA Quick Start

### Parameters

You can set the following parameters when running **Quickstart** to control its behavior:

* **-a** : Specify the type of audio that the demo sends for audio transmission test:
    * **1**: (Default) **OPUS**.
    * **3**: **WAV**.
    * **7**: **AACLC**.
* **-v** : Specify the type of video that the demo sends for video transmission test. 
    * **1**: **VP8**.
    * **2**: (Default) **H.264**.
* **-j** : Specify the degree of concurrency when sending a test, that is, the number of threads that send audio and video streams concurrently at the same time. The default value is 1.
* **-m** : Specify the content to be sent during the sending test.
    * **0**: Neither audio nor video.
    * **1**: Only video.
    * **2**: (Default) Only audio.
    * **3**: Both audio and video.
* **-n** : Specify the run of the test to be sent. The length of the audio test file or video test file used to send tests in the SDK Demo is tens of seconds to several minutes. This parameter is used to control the number of times these test files are sent. The default value is **1**.
* **-r** : Specify the demo to perform the receiving test. The demo performs the sending test by default. 
    * **0**: (Default) **pull mode** receives data
    * **1**: **observer mode** receive Data. 
* **-b** : Specify the form of sending and receiving DataStream by demo. 
    * **0**: (Default) not open.
    * **1**: **reliable/ordered**.
    * **2**: **unreliable/unordered**.
* **-d** : Used to specify the duration of the reception test, only useful when **-r**.
* **-u** : Used to specify the test userId. If there are multiple user tests, changes will be made based on this userId to generate other userIds.
* **-c** : Used to specify the test channel name. The default channel name is **conn_default_rtsa**.
* **-s ：** Used to receive data in the form of an observer, save the data to a file, the file format is wav. For **recording/playback/mixed** types saving, the default file name is **user_pcm_audio_data.wav**; For **before-mixed** type saving, the default file name is **uid + ${uid} + _user_pcm_audio_data.wav**。
    * **0**: saving **recording** data, i.e. agora::media::IAudioFrameObserver::onRecordAudioFrame receives(**RTSA2.0 not supported**)
    * **1**: saving **playback** data, i.e. agora::media::IAudioFrameObserver::onPlaybackAudioFrame receives
    * **2**: saving **before mixed** data, i.e. agora::media::IAudioFrameObserver::onPlaybackAudioFrameBeforeMixing receives
    * **3**: saving **mixed** data, i.e. agora::media::IAudioFrameObserver::onMixedAudioFrame receives(**RTSA2.0 not supported**)
* **-p** : Used to specify that Raw data is transmitted using Media Packet and Control Packet for audio and video, and the receiver can only use observer mode, that is, **-p -r 1**.

### examples

```
$ ./rtsa_quick_start -a 8 -c rtsa           # Send AAC test file in a single round and single thread, the channel name is `rtsa`
$ ./rtsa_quick_start -j 2 -m 1              # Send video concurrently with 2 threads
$ ./rtsa_quick_start -n 2                   # Tested twice
$ ./rtsa_quick_start -r 0 -d 10000          # Receive 10 seconds test data in pull mode, unit is millisecond
$ ./rtsa_quick_start -j 10 -u 10000         # 10 threads send audio and video concurrently, user Id is 10000, 10001, 10002 ... 10009
$ ./rtsa_quick_start -r 1 -j 5 -d 20000     # 5 users receive 20 seconds of test data in the form of an observer, in milliseconds
$ ./rtsa_quick_start -r 1 -s 1              # Receives data in the form of an observer and saves the file with the file name `user_pcm_audio_data.wav.wav`
$ ./rtsa_quick_start -r 2 -b 1              # Send and receive DataStream data in reliable / ordered 
```