//
//  IAgoraRecordingEngine.h
//  recording sdk
//
//  Created by HN on 2023/8/8.
//

#ifndef IAgoraRecordingEngine_h
#define IAgoraRecordingEngine_h

#include "IAgoraLinuxSdkCommon.h"
namespace agora {
namespace recording {


class IRecordingEngineEventHandler {

public:
    virtual ~IRecordingEngineEventHandler() {}

    /** Occurs when an error occurs during SDK runtime.
     *
     * The SDK cannot fix the issue or resume running. It requires intervention from the application and informs the user on the issue.
     *
     * @param error \ref ERROR_CODE_TYPE "Error codes".
     * @param stat_code \ref STAT_CODE_TYPE "State codes".
     */
    virtual void onError(int error, STAT_CODE_TYPE stat_code) {
        (void) error;
        (void) stat_code;
      }

    /** Occurs when a warning occurs during SDK runtime.
     *
     * In most cases, the application can ignore the warnings reported by the SDK because the SDK can usually fix the issue and resume running.
     *
     * @param warn \ref WARN_CODE_TYPE "Warning codes".
     */
    virtual void onWarning(int warn) {
        (void) warn;
      }

    /** Occurs when the recording server joins the channel.
     *
     * @param channelId Channel ID assigned based on the channel name specified in \ref IRecordingEngine::joinChannel "joinChannel".
     * @param uid The UID of the recording server.
     */
    virtual void onJoinChannelSuccess(const char * channelId, uid_t uid) {
        (void) channelId;
        (void) uid;
      }

    /** Occurs when the recording server leaves the channel.
     *
     * @param code The reasons why the recording server leaves the channel. See the \ref LEAVE_PATH_CODE "LEAVE_PATH_CODE".
     */
    virtual void onLeaveChannel(LEAVE_PATH_CODE code) {
        (void) code;
    }

    /** Occurs when a remote user/host joins the channel.
     *
     * - Communication profile: This callback notifies the recording server that a remote user joins the channel and reports the user's UID and information.
     * - Live broadcast profile: This callback notifies the recording server that a host joins the channel and reports the user's UID and information.
     *
     * If there are users/hosts in the channel before the recording server joins the channel, the SDK also reports on the UIDs and information of the existing users/hosts. This callback is triggered as many times as the number of the users/hosts in the channel.
     *
     * @param uid The UID of the remote user/host joining the channel.
     * @param infos \ref UserJoinInfos "User information".
     */
    virtual void onUserJoined(uid_t uid)  {
        (void) uid;
    }

    /** Occurs when the state of a remote user's video stream changes.
     *
     * @param uid The UID of the remote user.
     * @param state Indicates the current state of the remote user's video stream. For details, see \ref RemoteStreamState "RemoteStreamState".
     * @param reason Indicates the reason causing the state change. For details, see \ref RemoteStreamStateChangedReason "RemoteStreamStateChangedReason".
     */
    virtual void onRemoteVideoStreamStateChanged(uid_t uid, RemoteStreamState state, RemoteStreamStateChangedReason reason)   {
        (void) state;
        (void) reason;
    }

    /** Occurs when the state of a remote user's audio stream changes.
     *
     * @param uid The UID of the remote user.
     * @param state Indicates the current state of the remote user's audio stream. For details, see \ref RemoteStreamState "RemoteStreamState".
     * @param reason Indicates the reason causing the state change. For details, see \ref RemoteStreamStateChangedReason "RemoteStreamStateChangedReason".
     */
    virtual void onRemoteAudioStreamStateChanged(uid_t uid, RemoteStreamState state, RemoteStreamStateChangedReason reason) {
        (void) uid;
        (void) state;
        (void) reason;
    }

    /** Occurs when a user leaves the channel or goes offline.
     *
     * When no data package of a user is received for a certain period of time (15 seconds), the SDK assumes that the user has goes offline. Weak network connections may lead to misinformation, so Agora recommends using the signaling system for offline event detection.
     *
     * @param uid The user ID.
     * @param reason The \ref USER_OFFLINE_REASON_TYPE "reasons" why the user leaves the channel or goes offline.
     */
    virtual void onUserOffline(uid_t uid, USER_OFFLINE_REASON_TYPE reason) {
        (void) uid;
        (void) reason;
    }

    /** Reports the user who speaks loudest.
     *
     * If you set the \ref agora::recording::RecordingConfig::audioIndicationInterval "audioIndicationInterval" parameter in RecordingConfig to be greater than 0, this callback returns the `uid` of the user with the highest volume over a certain time period.
     *
     * @param uid The UID of the user with the highest volume over a certain time period.
     */
    virtual void onActiveSpeaker(uid_t uid)  {
        (void) uid;
    }

    /** Reports the list of users who are speaking and their volumes.
     *
     * This callback works only when the \ref agora::recording::RecordingConfig::audioIndicationInterval "audioIndicationInterval" parameter in RecordingConfig is set to be greater than 0.
     *
     * @param speakers    An array containing the user ID and volume information for each speaker. For more information, see \ref AudioVolumeInfo "AudioVolumeInfo".
     * @param speakerNum  The total number of users who are speaking.
     */
    virtual void onAudioVolumeIndication(const AudioVolumeInfo* speakers, unsigned int speakerNum)  {
        (void) speakers;
        (void) speakerNum;
    }

    /** Occurs when the first remote video frame is decoded.
     *
     * This callback is triggered when the first frame of the remote video is received and decoded.
     *
     * @param uid     The user ID.
     * @param width   The width of the video frame.
     * @param height  The height of the video frame.
     * @param elapsed Time elapsed (ms) from the local user calling \ref IRecordingEngine::joinChannel "joinChannel" until this callback is triggered.
     */
    virtual void onFirstRemoteVideoDecoded(uid_t uid, int width, int height, int elapsed) {
        (void) uid;
        (void) width;
        (void) height;
        (void) elapsed;
    }

    /** Occurs when the first remote audio frame is received.
     *
     * @param uid      The user ID.
     * @param elapsed  Time elapsed (ms) from the local user calling \ref IRecordingEngine::joinChannel "joinChannel" until this callback is triggered.
     */
    virtual void onFirstRemoteAudioFrame(uid_t uid, int elapsed) {
        (void) uid;
        (void) elapsed;
    }

    /** Occurs when the status of receiving the audio or video stream changes.
     *
     * @param receivingAudio  Whether or not the recording server is receiving the audio stream.
     * @param receivingVideo  Whether or not the recording server is receiving the video stream.
     */
    virtual void onReceivingStreamStatusChanged(bool receivingAudio, bool receivingVideo) {
        (void) receivingAudio;
        (void) receivingVideo;
    }

    /** Occurs when the SDK cannot reconnect to Agora's edge server 10 seconds after its connection to the server is interrupted.
     *
     * The SDK triggers this callback when it cannot connect to the server 10 seconds after calling \ref IRecordingEngine::joinChannel "joinChannel", regardless of whether it is in the channel or not.
     *
     * This callback is different from \ref IRecordingEngineEventHandler::onConnectionInterrupted "onConnectionInterrupted":
     *
     * - The SDK triggers the \ref IRecordingEngineEventHandler::onConnectionInterrupted "onConnectionInterrupted" callback when the SDK loses connection with the server for more than 4 seconds after it joins the channel.
     * - The SDK triggers the \ref IRecordingEngineEventHandler::onConnectionLost "onConnectionLost" callback when the SDK loses connection with the server for more than 10 seconds, regardless of whether it joins the channel or not.
     *
     * For both callbacks, the SDK tries to reconnect to the server until the app calls \ref IRecordingEngine::leaveChannel "leaveChannel".
     */
    virtual void onConnectionLost() {
    }

    /** Occurs when the connection between the SDK and the server is interrupted.
     *
     * The SDK triggers this callback when it loses connection to the server for more than 4 seconds after the connection is established. After triggering this callback, the SDK tries to reconnect to the server. You can use this callback to implement pop-up reminders.
     *
     * This callback is different from \ref IRecordingEngineEventHandler::onConnectionInterrupted "onConnectionInterrupted":
     *
     * - The SDK triggers the \ref IRecordingEngineEventHandler::onConnectionInterrupted "onConnectionInterrupted" callback when the SDK loses connection with the server for more than 4 seconds after it joins the channel.
     * - The SDK triggers the \ref IRecordingEngineEventHandler::onConnectionLost "onConnectionLost" callback when the SDK loses connection with the server for more than 10 seconds, regardless of whether it joins the channel or not.
     *
     * For both callbacks, the SDK tries to reconnect to the server until the app calls \ref IRecordingEngine::leaveChannel "leaveChannel".
     */
    virtual void onConnectionInterrupted() {
    }


    /** Occurs when the recording server rejoins the channel after being disconnected due to network problems.
     *
     * When the recording server loses connection with the server because of network problems, the SDK automatically tries to reconnect and triggers this callback upon reconnection.
     *
     * @param channelId The channel name.
     * @param uid The UID of the recording server.
     */
    virtual void onRejoinChannelSuccess(const char* channelId, uid_t uid) {
        (void) channelId;
        (void) uid;
    }

    /** Occurs when the network connection state changes.
     *
     * @param state The current network connection state. For details, see \ref ConnectionStateType "ConnectionStateType".
     * @param reason The reason causing the change of the connection state. For details, see \ref ConnectionChangedReasonType "ConnectionChangedReasonType".
     */
    virtual void onConnectionStateChanged(ConnectionStateType state, ConnectionChangedReasonType reason) {
        (void) state;
        (void) reason;
    }

    /** Reports the statistics of the video stream
     * from the remote user(communication profile)/host (live broadcast profile).
     *
     * The SDK triggers this callback once every two seconds
     * for each remote (communication profile)/host (live broadcast profile).
     * If a channel includes multiple remote users/hosts, the SDK triggers this
     * callback as many times.
     *
     * @param uid The UID of the user sending the video stream.
     * @param stats The statistics of the received remote video stream. See \ref RemoteVideoStats "RemoteVideoStats".
     */
    virtual void onRemoteVideoStats(uid_t uid, const RemoteVideoStats& stats) {
        (void) uid;
        (void) stats;
    }

    /** Reports the statistics of the audio stream
     * from the remote user(communication profile)/host (live broadcast profile).
     *
     * The SDK triggers this callback once every two seconds
     * for each remote (communication profile)/host (live broadcast profile).
     * If a channel includes multiple remote users, the SDK triggers this
     * callback as many times.
     *
     * @param uid The UID of the user sending the audio stream.
     * @param stats The statistics of the received remote audio stream. See \ref RemoteAudioStats "RemoteAudioStats".
     */
    virtual void onRemoteAudioStats(uid_t uid, const RemoteAudioStats& stats) {
        (void) uid;
        (void) stats;
    }

    /** Reports the statistics of \ref agora::recording::IRecordingEngine "IRecordingEngine" once every two seconds.
     *
     * @param stats See \ref RecordingStats "RecordingStats".
     */
    virtual void onRecordingStats(const RecordingStats& stats) {
        (void) stats;
    }

};


typedef struct RecordingConfig {
    /** Sets whether to record the streams of all users or specified users.
     * - true: (Default) Record the streams of all users.
     * - false: Record the streams of specified users.
     *
     * @note If you set #autoSubscribe as false, you should set #subscribeVideoUids
     * or #subscribeAudioUids to specify users whose video or audio you want to record.
     */
    bool autoSubscribe;
    /** If you set #isMixingEnabled as true and enable composite recording mode,
     * #mixResolution allows you to set the video profile, including the width,
     * height, frame rate, and bitrate. The default setting is 360 x 640, 15 fps, 500 Kbps.
     *
     * @note Agora supports the following frame rates: 1 fps, 7 fps, 10 fps, 15 fps,
     * 24 fps, 30 fps, and 60 fps. The default value is 15 fps.
     * If you set other frame rates, the SDK uses the default value.
     *
     * See the [Video Profile Table](https://docs.agora.io/en/faq/recording_video_profile).
     */
    rtc::VideoEncoderConfiguration mixResolution;
    /** Sets the path of the recorded files. The default value is NULL.

     After setting `recordFileRootDir`, the subdirectory will be automatically generated according to the date of the recording.
     */
    const char * recordFileRootDir;
    /** Sets a time period. The value must be &ge; 3 seconds. The default value is 300 seconds.

     When the Agora Recording SDK is recording, if there is no user in the channel after a time period of `idleLimitSec`, it automatically stops recording and leaves the channel.

     @note
     - We charge you this time period.
     - In a communication channel, the recording service does not recognize a channel as an idle channel, so long as the channel has users, regardless of whether they send stream or not.
     - If a live streaming channel has an audience without a host for a set time (`idleLimitSec`), the recording service automatically stops and leaves the channel.
     */
    int idleLimitSec;
    /** Sets whether or not to detect the users who speak.

     - &le; 0: (Default) Do not detect the users who speak.
     - > 0: Sets the interval (ms) of detecting the users who speak. Agora recommends setting the interval to be longer than 200 ms. When the SDK detects the users who speak, the SDK returns the UID of the user who speaks loudest in the \ref agora::recording::IRecordingEngineEventHandler::onActiveSpeaker "onActiveSpeaker" callback and returns the UIDs of all users who speak and their voice volumes in the \ref agora::recording::IRecordingEngineEventHandler::onAudioVolumeIndication "onAudioVolumeIndication" callback.
     */
    int audioIndicationInterval;
    /** Sets the channel mode. For more information, see \ref CHANNEL_PROFILE_TYPE "CHANNEL_PROFILE_TYPE". */
    CHANNEL_PROFILE_TYPE channelProfile;
    /** Takes effect only when the Agora Native SDK enables the dual-stream mode (high stream by default). For more information, see \ref REMOTE_VIDEO_STREAM_TYPE "REMOTE_VIDEO_STREAM_TYPE".
     */
    REMOTE_VIDEO_STREAM_TYPE streamType;

    AUDIO_PROFILE_TYPE audioProfile;

    RecordingConfig():
        autoSubscribe(true),
        mixResolution(),
        recordFileRootDir(NULL),
        audioIndicationInterval(0),
        channelProfile(CHANNEL_PROFILE_COMMUNICATION),
        streamType(REMOTE_VIDEO_STREAM_HIGH)
    {}

    virtual ~RecordingConfig() {}
} RecordingConfig;

class IRecordingEngine {
public:

    /** This method creates an \ref agora::recording::IRecordingEngine "IRecordingEngine" instance.

     @param appId The App ID used in the communications to be recorded. For more information
     @param eventHandler The Agora Recording SDK notifies the application of the triggered events by callbacks in the \ref agora::recording::IRecordingEngineEventHandler "IRecordingEngineEventHandler".

     @return An \ref agora::recording::IRecordingEngine "IRecordingEngine" instance.
     */
    static IRecordingEngine* createAgoraRecordingEngine(const char * appId, IRecordingEngineEventHandler *eventHandler);

    virtual ~IRecordingEngine() {}

    /** This method allows the recording server to join a channel and start recording.

     @note
     - In the Recording SDK, `requestToken` and `renewToken` are private methods. Make sure that you set [privilegeExpiredTs](https://docs.agora.io/en/Recording/token_server) as 0 when generating a token, which means that the privilege, once generated, never expires.
     - A channel does not accept duplicate uids. Otherwise, there will be unpredictable behaviors.

     @param channelKey The `channelKey` for authentication. Set `channelKey` of the recording server the same as that of the Native/Web SDK. If the Native/Web SDK uses a token, `channelKey` must be set as the token. For more information, see [Use Security Keys](https://docs.agora.io/en/Voice/token?platform=All%20Platforms#app-id-native).
     @param channelId  The name of the channel to be recorded.

     @param uid        The unique identifier of the recording server.
     - If you set `uid` as 0, the SDK randomly assigns a uid and returns it in the \ref IRecordingEngineEventHandler::onJoinChannelSuccess "onJoinChannelSuccess" callback.
     - If you set your own `uid`, it should be a 32-bit unsigned integer ranging from 1 to (2<sup>32</sup>-1).

     @param config     Detailed recording configuration. See \ref agora::recording::RecordingConfig "RecordingConfig".

     @return
     - 0: Success.
     - < 0: Failure.

     */
    virtual int joinChannel(const char * channelKey, const char *channelId, uid_t uid, const RecordingConfig &config) = 0;

    /** This method sets the video layout in composite recording mode.
     *
     * @note If you record video in composite recording mode, you must call this
     * method to set the video layout.
     *
     * @param layout Layout setting. See \ref VideoMixingLayout "VideoMixingLayout".
     *
     * @return
     * - 0: Success.
     * - < 0: Failure.
     */
    //virtual int setVideoMixingLayout(const VideoMixingLayout &layout) = 0;

    /** This method allows the recording server to leave the channel and release the thread resources.

     @return
     - 0: Success.
     - < 0: Failure.
     */
    virtual int leaveChannel() = 0;

    /** This method destroys the \ref agora::recording::IRecordingEngine "IRecordingEngine" instance.

     @return
     - 0: Success.
     - < 0: Failure.
     */
    virtual int release() = 0;

    /** This method manually starts recording.

     This method is valid only when you set \ref agora::recording::RecordingConfig::triggerMode "triggerMode" in RecordingConfig as 1 (manually).

     @return
     - 0: Success.
     - < 0: Failure.
     */
    virtual int startService(const char * channelKey, const char *channelId, uid_t uid) = 0;

    /** This method manually stops recording.

     This method is valid only when you set \ref agora::recording::RecordingConfig::triggerMode "triggerMode" in RecordingConfig as 1 (manually).

     @return
     - 0: Success.
     - < 0: Failure.
     */
    virtual int stopService() = 0;

    /** This method sets the log level.

     Only log levels preceding the selected level are generated. The default value of the log level is 5.

     @param level See \ref agora_log_level "agora_log_level".
     @return
     - 0: Success.
     - < 0: Failure.
     */
    //virtual int setLogLevel(agora_log_level level) = 0;

    /** This method updates the UIDs of the users whose video streams you want to record.
     *
     * @note Ensure that you set the \ref agora::recording::RecordingConfig::autoSubscribe "autoSubscribe" parameter in the \ref agora::recording::RecordingConfig "RecordingConfig" as false before calling this method.
     *
     * @param uids An array of UIDs whose video streams you want to record
     *             in string format, such as `{"1","2","3"}`.
     * @param num The number of UIDs.
     *
     * @return
     * - 0: Success.
     * - < 0: Failure.
     */
    //virtual int updateSubscribeVideoUids(uid_t *uids, uint32_t num) = 0;

    /** This method updates the UIDs of the users whose audio streams you want to record.
     *
     * @note Ensure that you set the \ref agora::recording::RecordingConfig::autoSubscribe "autoSubscribe" parameter in the \ref agora::recording::RecordingConfig "RecordingConfig" as false before calling this method.
     *
     * @param uids An array of UIDs whose audio streams you want to record
     *             in string format, such as `{"1","2","3"}`.
     * @param num The number of UIDs.
     *
     * @return
     * - 0: Success.
     * - < 0: Failure.
     */
    //virtual int updateSubscribeAudioUids(uid_t *uids, uint32_t num) = 0;
};

}
}
#endif /* IAgoraRecordingEngine_h */
