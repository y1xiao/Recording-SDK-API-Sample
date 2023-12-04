cmake_minimum_required(VERSION 3.0)
project(license_tools)

include_directories(../third-party/http_parser/include)
include_directories(../third-party/json_parser/include)

aux_source_directory(. DIR_SRCS)

add_executable(license_activator license_activator.cpp)
target_link_libraries(license_activator agora_rtc_sdk http_parser)

add_executable(license_query license_query.cpp)
target_link_libraries(license_query http_parser)
