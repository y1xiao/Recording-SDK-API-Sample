cmake_minimum_required(VERSION 3.5)
project(DefaultSamples)

link_directories("${PROJECT_SOURCE_DIR}/../../../../sdk")

# Common file parsers
file(GLOB FILE_PARSER_CPP_FILES
     "${PROJECT_SOURCE_DIR}/../common/file_parser/helper_h264_parser.cpp"
     "${PROJECT_SOURCE_DIR}/../common/file_parser/helper_aac_parser.cpp")

# Opus file parser
file(GLOB OPUS_FILE_PARSER_CPP_FILES
     "${PROJECT_SOURCE_DIR}/../common/file_parser/helper_opus_parser.cpp")

# Build sample_send_yuv_pcm
file(GLOB SAMPLE_LOCAL_RECORDER
     "${PROJECT_SOURCE_DIR}/recorder_local.cpp"
     "${PROJECT_SOURCE_DIR}/../common/*.cpp")
add_executable(recorder_local ${SAMPLE_LOCAL_RECORDER})
#target_link_libraries(recorder_local agora-soundtouch)
target_link_libraries(recorder_local "-Wl,--disable-new-dtags")
#set(CMAKE_INSTALL_RPATH "${PROJECT_SOURCE_DIR}/../../../../sdk")
#set_target_properties(recorder_local PROPERTIES INSTALL_RPATH "${CMAKE_INSTALL_RPATH}")
# Build sample_send_nvqsv
#file(GLOB SAMPLE_SEND_NVQSV_CPP_FILES
#     "${PROJECT_SOURCE_DIR}/sample_send_nvqsv.cpp"
#     "${PROJECT_SOURCE_DIR}/../common/*.cpp")
#add_executable(sample_send_nvqsv ${SAMPLE_SEND_NVQSV_CPP_FILES})

# Build sample_send_yuv_dual_stream
#file(GLOB SAMPLE_SEND_YUV_DUAL_STREAM_FILES
#     "${PROJECT_SOURCE_DIR}/sample_send_yuv_dual_stream.cpp"
#     "${PROJECT_SOURCE_DIR}/../common/*.cpp")
#add_executable(sample_send_yuv_dual_stream ${SAMPLE_SEND_YUV_DUAL_STREAM_FILES})

# Build sample_send_h264_opus
#file(GLOB SAMPLE_SEND_H264_OPUS_CPP_FILES
#     "${PROJECT_SOURCE_DIR}/sample_send_h264_opus.cpp"
#     "${PROJECT_SOURCE_DIR}/../common/*.cpp")
#add_executable(
#  sample_send_h264_opus ${SAMPLE_SEND_H264_OPUS_CPP_FILES}
#                        ${FILE_PARSER_CPP_FILES} ${OPUS_FILE_PARSER_CPP_FILES})
#target_compile_definitions(sample_send_h264_opus PRIVATE __SUPPORT_OPUS__)
#target_link_libraries(sample_send_h264_opus opusfile_static)

# Build sample_send_h264_pcm
#file(GLOB SAMPLE_SEND_H264_PCM_CPP_FILES
#     "${PROJECT_SOURCE_DIR}/sample_send_h264_pcm.cpp"
#     "${PROJECT_SOURCE_DIR}/../common/*.cpp")
#add_executable(sample_send_h264_pcm ${SAMPLE_SEND_H264_PCM_CPP_FILES}
#                                    ${FILE_PARSER_CPP_FILES})

# Build sample_stringuid_send
#file(GLOB SAMPLE_STRINGUID_SEND_FILES
#     "${PROJECT_SOURCE_DIR}/sample_stringuid_send.cpp"
#     "${PROJECT_SOURCE_DIR}/../common/*.cpp")
#add_executable(sample_stringuid_send ${SAMPLE_STRINGUID_SEND_FILES}
#                                    ${FILE_PARSER_CPP_FILES})


# Build sample_send_encrypted_h264
#file(GLOB SAMPLE_SEND_ENCRYPTED_H264_CPP_FILES
#     "${PROJECT_SOURCE_DIR}/sample_send_encrypted_h264.cpp"
#     "${PROJECT_SOURCE_DIR}/../common/*.cpp")
#add_executable(sample_send_encrypted_h264 ${SAMPLE_SEND_ENCRYPTED_H264_CPP_FILES}
#                                    ${FILE_PARSER_CPP_FILES})

# Build sample_send_aac
#file(GLOB SAMPLE_SEND_AAC_CPP_FILES "${PROJECT_SOURCE_DIR}/sample_send_aac.cpp"
#     "${PROJECT_SOURCE_DIR}/../common/*.cpp")
#add_executable(sample_send_aac ${SAMPLE_SEND_AAC_CPP_FILES}
#                               ${FILE_PARSER_CPP_FILES})

# Build sample_receive_yuv_pcm
#file(GLOB SAMPLE_RECEIVE_YUV_PCM_CPP_FILES
#     "${PROJECT_SOURCE_DIR}/sample_receive_yuv_pcm.cpp"
#     "${PROJECT_SOURCE_DIR}/../common/*.cpp")
#add_executable(sample_receive_yuv_pcm ${SAMPLE_RECEIVE_YUV_PCM_CPP_FILES})

# Build sample_receive_decrypted_h264
#file(GLOB SAMPLE_RECEIVE_DECRYPTED_H264_CPP_FILES
#     "${PROJECT_SOURCE_DIR}/sample_receive_decrypted_h264.cpp"
#     "${PROJECT_SOURCE_DIR}/../common/*.cpp")
#add_executable(sample_receive_decrypted_h264 ${SAMPLE_RECEIVE_DECRYPTED_H264_CPP_FILES})

# Build sample_receive_yuv_from_multi_user
#file(GLOB SAMPLE_RECEIVE_YUV_FROM_MULTI_USER_CPP_FILES
#     "${PROJECT_SOURCE_DIR}/sample_receive_yuv_from_multi_user.cpp"
#     "${PROJECT_SOURCE_DIR}/../common/*.cpp")
#add_executable(sample_receive_yuv_from_multi_user ${SAMPLE_RECEIVE_YUV_FROM_MULTI_USER_CPP_FILES})

# Build sample_receive_mixed_audio
#file(GLOB SAMPLE_RECEIVE_MIXED_AUDIO_FILES
#     "${PROJECT_SOURCE_DIR}/sample_receive_mixed_audio.cpp"
#     "${PROJECT_SOURCE_DIR}/../common/*.cpp")
#add_executable(sample_receive_mixed_audio ${SAMPLE_RECEIVE_MIXED_AUDIO_FILES})



# Build sample_receive_h264_pcm
#file(GLOB SAMPLE_RECEIVE_H264_PCM_CPP_FILES
#     "${PROJECT_SOURCE_DIR}/sample_receive_h264_pcm.cpp"
#     "${PROJECT_SOURCE_DIR}/../common/*.cpp")
#add_executable(sample_receive_h264_pcm ${SAMPLE_RECEIVE_H264_PCM_CPP_FILES})

# Build sample_stringuid_receive
#file(GLOB SAMPLE_STRINGUID_RECEIVE
#     "${PROJECT_SOURCE_DIR}/sample_stringuid_receive.cpp"
#     "${PROJECT_SOURCE_DIR}/../common/*.cpp")
#add_executable(sample_stringuid_receive ${SAMPLE_STRINGUID_RECEIVE})

# Build sample_send_h264_multi_inst
#file(GLOB SAMPLE_SEND_H264_MULTI_INST_CPP_FILES
#     "${PROJECT_SOURCE_DIR}/sample_send_h264_multi_inst.cpp"
#     "${PROJECT_SOURCE_DIR}/../common/*.cpp")
#add_executable(
#  sample_send_h264_multi_inst ${SAMPLE_SEND_H264_MULTI_INST_CPP_FILES}
#                              ${FILE_PARSER_CPP_FILES})

# Build sample_send_receive_pcm_multi_inst
#file(GLOB SAMPLE_SEND_RECEIVE_PCM_MULTI_INST_CPP_FILES
#     "${PROJECT_SOURCE_DIR}/sample_send_receive_pcm_multi_inst.cpp"
#     "${PROJECT_SOURCE_DIR}/../common/*.cpp")
#add_executable(sample_send_receive_pcm_multi_inst
#               ${SAMPLE_SEND_RECEIVE_PCM_MULTI_INST_CPP_FILES})

# Build sample_send_generic
#file(GLOB SAMPLE_SEND_GENERIC_CPP_FILES
#     "${PROJECT_SOURCE_DIR}/sample_send_generic.cpp"
#     "${PROJECT_SOURCE_DIR}/../common/*.cpp")
#add_executable(sample_send_generic ${SAMPLE_SEND_GENERIC_CPP_FILES}
#                                   ${FILE_PARSER_CPP_FILES})

# Build sample_send_h264_dual_stream
#file(GLOB SAMPLE_SEND_H264_DUAL_STREAM_CPP_FILES
#     "${PROJECT_SOURCE_DIR}/sample_send_h264_dual_stream.cpp"
#     "${PROJECT_SOURCE_DIR}/../common/*.cpp")
#add_executable(sample_send_h264_dual_stream ${SAMPLE_SEND_H264_DUAL_STREAM_CPP_FILES}
#                                    ${FILE_PARSER_CPP_FILES})

