project(TinyEmberPlus)

if(NOT MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-unused-local-typedefs")
endif()

set(QT_USE_QTGUI TRUE)
set(QT_USE_QTXML TRUE)
set(QT_USE_QTNETWORK TRUE)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package( Qt5Core )
find_package( Qt5Gui )
find_package( Qt5Widgets )
find_package( Qt5Network )
find_package( Qt5Xml )

include_directories ( ${Qt5Core_INCLUDE_DIRS} )
include_directories ( ${Qt5Gui_INCLUDE_DIRS} )
include_directories ( ${Qt5Widgets_INCLUDE_DIRS} )
include_directories ( ${Qt5Network_INCLUDE_DIRS} )
include_directories ( ${Qt5Xml_INCLUDE_DIRS} )

include_directories( ${CMAKE_BINARY_DIR}
        ${CMAKE_CURRENT_BINARY_DIR} # generated ui_.h files
        ../../libember/Headers
        ../../libformula/Headers
        ../../libs101/Headers)

file(GLOB HEADER_FILES
        gadget/*.h
        gadget/util/*.h
        glow/*.h
        glow/util/*.h
        net/*.h
        serialization/*.h
        serialization/detail/*.h
        util/*.h
        *.h)

file(GLOB SOURCE_FILES
        gadget/*.cpp
        gadget/util/*.cpp
        glow/*.cpp
        glow/util/*.cpp
        net/*.cpp
        serialization/*.cpp
        serialization/detail/*.cpp
        util/*.cpp
        *.cpp
        )

set ( GLOB MOC_HEADERS
        ui_*.h
        gadget/*.h
        gadget/util/*.h
        glow/*.h
        glow/util/*.h
        net/*.h
        serialization/*.h
        serialization/detail/*.h
        util/*.h
        *.h
        gadget/*.cpp
        gadget/util/*.cpp
        glow/*.cpp
        glow/util/*.cpp
        net/*.cpp
        serialization/*.cpp
        serialization/detail/*.cpp
        util/*.cpp
        *.cpp
        )

set ( GLOB UIS
        *.ui
        )

set ( GLOB RESOURCES
        main.qrc
        )

qt5_add_resources( RES_SOURCES ${RESOURCES} )
qt5_wrap_ui( UI_HEADERS ${UIS} )
qt5_wrap_cpp( MOC_SRCS ${MOC_HEADERS})

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${MOC_HEADERS} ${RES_SOURCES} ${UI_HEADERS})
target_link_libraries(${PROJECT_NAME} ember_static ${Qt5Core_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ember_static ${Qt5Gui_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ember_static ${Qt5Widgets_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ember_static ${Qt5Network_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ember_static ${Qt5Xml_LIBRARIES})
