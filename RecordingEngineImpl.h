//
//  RecordingEngineImpl.h
//  recording sdk
//
//  Created by HN on 2023/8/8.
//

#ifndef RecordingEngineImpl_h
#define RecordingEngineImpl_h


#include "IAgoraLinuxSdkCommon.h"
#include "IAgoraRecordingEngine.h"
#include "subscriber_connection_observer.hpp"
#include "publisher_connection_observer.hpp"
#include "publisher_localuser_observer.hpp"
#include "subscriber_localuser_observer.hpp"
#include "NGIAgoraVideoMixerSource.h"
#include "common/log.h"
#include <string>
namespace agora {
namespace recording {


class RecordingEngineImpl : public IRecordingEngine {
public:
    static IRecordingEngine* createAgoraRecordingEngine(const char * appId, IRecordingEngineEventHandler *eventHandler);

    explicit RecordingEngineImpl(const char * appId, IRecordingEngineEventHandler *eventHandler);
    
    virtual ~RecordingEngineImpl();

    virtual int joinChannel(const char * channelKey, const char *channelId, uid_t uid, const RecordingConfig &config);

    virtual int leaveChannel();

    virtual int release();
    
    virtual int startService(const char * channelKey, const char *channelId, uid_t uid);
    
    virtual int stopService();
    
private:
    
    agora::base::IAgoraService* createAndInitAgoraService(bool enableAudioDevice,
                                                          bool enableAudioProcessor, bool enableVideo,bool enableuseStringUid = false,bool enablelowDelay = false, const char* appid =nullptr);
    
    std::string m_appid;
    IRecordingEngineEventHandler *m_eventHandler;
    agora::base::IAgoraService* m_service;
    agora::agora_refptr<agora::rtc::IRtcConnection> subscribeConnection;
    agora::agora_refptr<agora::rtc::IRtcConnection> mixedConnection;
    agora::agora_refptr<agora::rtc::IVideoMixerSource> m_videoMixer;
    agora::agora_refptr<agora::rtc::ILocalVideoTrack> m_mixVideoTrack;
    agora::agora_refptr<agora::rtc::IMediaMuxer> m_videoMuxer;
    std::shared_ptr<SubscriberConnectionObserver> m_subConnObserver;
    std::shared_ptr<SubscriberLocalUserObserver> m_subUserObserver;
    std::shared_ptr<EncodedFrameObserver> m_encodedFrameObserver;
    std::shared_ptr<MediaRecorderObserver> m_mediaRecorderObserver;
};
}
}
#endif /* RecordingEngineImpl_h */
