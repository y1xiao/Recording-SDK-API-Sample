# 录制 API Sample 简介
API Samples是基于agora sdk api完成的一个命令行工具，目标是为了对齐老版本（官网版本）的API，方便使用老版本SDK的用户升级。

# 准备工作
确保已经获取了对应的工程版本agora sdk

# 快速集成 Recording SDK
1.将agora sdk置于sdk 文件夹下
sdk目录下应该有以下6个文件

high_level_api文件夹
low_level_api文件夹
libagora-fdkaac.so
libagora-ffmpeg.so
libagora_rtc_sdk.so
libagora_soundtouch.so

2.到demo/agora_sdk_demo目录下，运行build_linux.sh命令（如果运行成功，应该可以在看到当前目录下有build_linux/bin/x86_64/recorder_local可执行文件）

# 开始录制
./recorder_local --appId {你的 App ID} --channel {频道名} --uid {Uid} --channelProfile {0 通信模式，1 直播模式} --appliteDir 

--Recording_Dir {录制文件所在目录绝对路径}
（这里无需再设置json文件）

--useStringUid {0 不使用string uid功能，1 使用string uid功能}
（String Uid支持，相比于老版本的SDK,uid参数可以使用string uid，在接收到来自string uid的流后，也会对用户进行录制，并以string uid对录制文件命名）

--treatStringUidAsIntUid {0 agora分配int uid, 1 自定义int uid}


# 合并音视频
录制过程中默认使用录制单个音视频的方式录制，视频为h264，音频为aac的情况下，容器格式为mp4，无须事后使用脚本合并音视频
文件截断只有在用户15秒内同时没有音视频流，或者用户离开频道超过15秒后截断
