
################################### Metadata ###################################
cmake_minimum_required(VERSION 3.9 FATAL_ERROR)


# Enable sane rpath handling on macOS
cmake_policy(SET CMP0042 NEW)
# Allow version in project definition
cmake_policy(SET CMP0048 NEW)
# Allow visibility definitions
cmake_policy(SET CMP0063 NEW)
# Allow interprocedural optimization
cmake_policy(SET CMP0069 NEW)

project(TinyEmberPlusRouter VERSION 1.5.1 LANGUAGES CXX)

# Use GNUInstallDirs to make sure libraries are installed into correct locations
# on all platforms.
include(GNUInstallDirs)

################################### Options ####################################


################################# Main Project #################################

include(cmake/modules/EnableWarnings.cmake)

# <<<  Build  >>>

# If libs101 is not already defined (e.g. because this is the toplevel invocation),
# look it up via find_package.
if(NOT TARGET libs101::s101)
    find_package(libs101 REQUIRED)
endif()


# If libformula is not already defined (e.g. because this is the toplevel invocation),
# look it up via find_package.
if(NOT TARGET libformula::formula)
    find_package(libformula REQUIRED)
endif()


# Choose whether to link against the static or the shared variant of libember
# depending on the preference specified by the invocation.
if (NOT BUILD_SHARED_LIBS)
    set(LIBEMBER_TARGET libember::ember-static)
else()
    set(LIBEMBER_TARGET libember::ember-shared)
endif()

# If libember is not already defined (e.g. because this is the toplevel invocation),
# look it up via find_package.
if(NOT TARGET ${LIBEMBER_TARGET})
    find_package(libember REQUIRED)
endif()


find_package(Qt5 COMPONENTS Core Network REQUIRED)


file(GLOB_RECURSE SOURCE_FILES
        TinyEmberPlusRouter/*.cpp
    )

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_compile_features(${PROJECT_NAME}
        PRIVATE
            cxx_std_11
    )
set_target_properties(${PROJECT_NAME}
        PROPERTIES
            AUTOMOC                      ON
            POSITION_INDEPENDENT_CODE    ON
            C_EXTENSIONS                 OFF
            CXX_EXTENSIONS               OFF
    )
target_link_libraries(${PROJECT_NAME}
        PRIVATE
            Qt5::Core
            Qt5::Network
            libs101::s101
            libformula::formula
            ${LIBEMBER_TARGET}
        )
enable_warnings_on_target(${PROJECT_NAME})


# Add the IPO property for all relevant targets, if we are building in the
# release configuration and the platform supports it.
if (NOT CMAKE_BUILD_TYPE MATCHES "Debug")
    if (NOT DEFINED check_ipo_supported)
        include(CheckIPOSupported)
        check_ipo_supported(RESULT ipo_supported)
    endif()

    if(ipo_supported)
        set_target_properties(${PROJECT_NAME} PROPERTIES INTERPROCEDURAL_OPTIMIZATION ON)
    endif()
endif()

# <<<  Install  >>>

install(TARGETS ${PROJECT_NAME}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

