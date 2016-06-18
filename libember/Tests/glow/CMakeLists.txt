cmake_minimum_required(VERSION 3.0)
project(libember_test_glow)
include_directories(../../Headers)
set(SOURCE_FILES
        GlowValue.cpp
        )
if( NOT CMAKE_BUILD_TYPE )
    set( CMAKE_BUILD_TYPE Debug CACHE STRING
            "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
            FORCE )
endif()
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ember_static)
