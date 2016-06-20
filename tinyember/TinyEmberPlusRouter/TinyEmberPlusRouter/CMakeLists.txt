project(TinyEmberPlusRouter)

if(NOT MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

set(QT_USE_QTXML TRUE)
set(QT_USE_QTNETWORK TRUE)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package( Qt4 REQUIRED QtCore QtNetwork)
include( ${QT_USE_FILE} )

include_directories( ${CMAKE_BINARY_DIR}
        ../../../libember/Headers
        ../../../libs101/Headers)


file(GLOB HEADER_FILES
        glow/*.h
        net/*.h
        model/*.h
        model/matrix/*.h
        model/matrix/detail/*.h
        util/*.h
        *.h)

file(GLOB SOURCE_FILES
        glow/*.cpp
        net/*.cpp
        model/*.cpp
        model/matrix/*.cpp
        model/matrix/detail/*.cpp
        util/*.cpp
        *.cpp
        )

set ( GLOB MOC_HEADERS
        glow/*.h
        net/*.h
        model/*.h
        model/matrix/*.h
        util/*.h
        *.h
        )

QT4_WRAP_CPP( MOC_SRCS ${MOC_HEADERS})
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${MOC_HEADERS})
target_link_libraries(${PROJECT_NAME} ember_static ${QT_LIBRARIES})

