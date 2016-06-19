cmake_minimum_required(VERSION 3.0.2)
project(libs101)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(${PROJECT_NAME}_INCLUDE_DIRS
        ${PROJECT_SOURCE_DIR}/Headers)

#set(SOURCE_FILES
#    Headers/s101/util/Crc16.hpp
#    Headers/s101/util/EnumCast.hpp
#    Headers/s101/Byte.hpp
#    Headers/s101/CommandType.hpp
#    Headers/s101/Dtd.hpp
#    Headers/s101/MessageType.hpp
#    Headers/s101/PackageFlag.hpp
#    Headers/s101/S101.hpp
#    Headers/s101/StreamDecoder.hpp
#    Headers/s101/StreamEncoder.hpp
#    Headers/s101/Version.hpp)

#add_library(libs101 STATIC ${SOURCE_FILES})
