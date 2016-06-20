project(libformula)

if(NOT MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

set ( ${PROJECT_NAME}_INCLUDE_DIRS
        ${PROJECT_SOURCE_DIR}/Headers
        ${PROJECT_SOURCE_DIR}/Headers/formula
        ${PROJECT_SOURCE_DIR}/Headers/formula/traits
        ${PROJECT_SOURCE_DIR}/Headers/formula/utils
        CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)