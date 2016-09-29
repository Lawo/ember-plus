cmake_minimum_required(VERSION 3.0.2)
project(libember_test_util)
include_directories(../../Headers)
set(SOURCE_FILES
        StreamBuffer.cpp
        )

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ember_static)
