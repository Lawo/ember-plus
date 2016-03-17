project(TinyEmberPlusRouter)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(Qt4 4.8.5 REQUIRED)

set(QT_USE_QTGUI TRUE)
set(QT_USE_QTXML TRUE)

include_directories(../../../libember/Headers
                    ../../../libs101/Headers)


file(GLOB HEADER_FILES
        glow/*.h
        net/*.h
        model/*.h
        model/matrix/*.h
        util/*.h
        *.h)

file(GLOB SOURCE_FILES
        glow/*.cpp
        net/*.cpp
        model/*.cpp
        model/matrix/*.cpp
        util/*.cpp
        *.cpp
        )

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

