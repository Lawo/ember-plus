cmake_minimum_required(VERSION 3.3)
project(libember_test_ber)
include_directories(../../Headers)

add_executable(DynamicEncodeDecode DynamicEncodeDecode.cpp)
target_link_libraries(DynamicEncodeDecode libember_static)

add_executable(StaticEncodeDecode StaticEncodeDecode.cpp)
target_link_libraries(StaticEncodeDecode libember_static)

add_executable(${PROJECT_NAME} StaticEncodeDecode.cpp)
target_link_libraries(${PROJECT_NAME} libember_static)