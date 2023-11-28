# 录制 API Sample 简介
API Samples是基于agora sdk api完成的一个命令行工具，直接以源码方式提供，目标是为了对齐老版本（官网版本）的API，方便使用老版本SDK的用户升级。

# 快速集成 Recording SDK准备工作
1.获取对应的agora sdk

1.将agora sdk置于libs 文件夹下的

2.运行make命令

3.recorder_local

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
