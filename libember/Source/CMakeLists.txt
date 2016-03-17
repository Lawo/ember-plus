project(ember)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories(../Headers)
set(${PROJECT_NAME}_INCLUDE_DIRS
        ${PROJECT_SOURCE_DIR}/Headers)
file(GLOB HEADER_FILES
        ../Headers/ember/ber/detail/*.hpp
        ../Headers/ember/ber/traits/*.hpp
        ../Headers/ember/ber/*.hpp
        ../Headers/ember/dom/detail/*.hpp
        ../Headers/ember/glow/traits/*.hpp
        ../Headers/ember/glow/util/traits/*.hpp
        ../Headers/ember/glow/util/*.hpp
        ../Headers/ember/glow/*.hpp
        ../Headers/ember/meta/*.hpp
        ../Headers/ember/util/detail/*.hpp
        ../Headers/ember/util/*.hpp
        ../Headers/ember/*.hpp
        )
file(GLOB SOURCE_FILES
        ber/*.cpp
        dom/*.cpp
        glow/*.cpp
        *.cpp)

add_library(${PROJECT_NAME}_static STATIC ${HEADER_FILES} ${SOURCE_FILES})
add_library(${PROJECT_NAME}_shared SHARED ${HEADER_FILES} ${SOURCE_FILES})