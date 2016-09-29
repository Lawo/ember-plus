cmake_minimum_required(VERSION 3.0.2)
project(libember_test_ber)
include_directories(../../Headers)

add_executable(DynamicEncodeDecode DynamicEncodeDecode.cpp)
target_link_libraries(DynamicEncodeDecode ember_static)

add_executable(StaticEncodeDecode StaticEncodeDecode.cpp)
target_link_libraries(StaticEncodeDecode ember_static)

add_executable(${PROJECT_NAME} StaticEncodeDecode.cpp)
target_link_libraries(${PROJECT_NAME} ember_static)
