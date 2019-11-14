
################################### Metadata ###################################
cmake_minimum_required(VERSION 3.9 FATAL_ERROR)


# Detect if we are invoked as the top level
if(NOT DEFINED PROJECT_NAME)
    set(IS_TOPLEVEL ON)
endif()

# Enable sane rpath handling on macOS
cmake_policy(SET CMP0042 NEW)
# Allow version in project definition
cmake_policy(SET CMP0048 NEW)
# Allow visibility definitions
cmake_policy(SET CMP0063 NEW)
# Allow interprocedural optimization
cmake_policy(SET CMP0069 NEW)

project(libember VERSION 1.8.2 LANGUAGES CXX)

# Use GNUInstallDirs to make sure libraries are installed into correct locations
# on all platforms.
include(GNUInstallDirs)

################################### Options ####################################


################################# Main Project #################################

include(cmake/modules/EnableWarnings.cmake)

# <<<  Build  >>>

add_library(ember-headers INTERFACE)
target_compile_features(ember-headers
        INTERFACE
            cxx_std_98
    )
target_include_directories(ember-headers 
        INTERFACE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Headers>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )

# Alias ember-headers to libember::ember-headers so that this library can be
# used in lieu of a module from the local source tree
add_library(${PROJECT_NAME}::ember-headers ALIAS ember-headers)


add_library(ember-headeronly INTERFACE)
target_compile_definitions(ember-headeronly
        INTERFACE
            LIBEMBER_HEADER_ONLY
    ) 
target_link_libraries(ember-headeronly
        INTERFACE
            libember::ember-headers
    )

# Alias ember-headeronly to libember::ember-headeronly so that this library can be
# used in lieu of a module from the local source tree
add_library(${PROJECT_NAME}::ember-headeronly ALIAS ember-headeronly)


file(GLOB_RECURSE SOURCE_FILES Source/*.cpp)


add_library(ember-shared SHARED ${SOURCE_FILES})
set_target_properties(ember-shared
        PROPERTIES
            POSITION_INDEPENDENT_CODE    ON
            VISIBILITY_INLINES_HIDDEN    ON
            C_VISIBILITY_PRESET          hidden
            CXX_VISIBILITY_PRESET        hidden
            C_EXTENSIONS                 OFF
            CXX_EXTENSIONS               OFF
    )
target_compile_definitions(ember-shared
        PUBLIC
            LIBEMBER_DLL
        PRIVATE
            LIBEMBER_DLL_EXPORTS
    ) 
target_link_libraries(ember-shared
        PUBLIC
            libember::ember-headers
    )
enable_warnings_on_target(ember-shared)

# Alias ember-shared to libember::ember-shared so that this library can be
# used in lieu of a module from the local source tree
add_library(${PROJECT_NAME}::ember-shared ALIAS ember-shared)


add_library(ember-static STATIC ${SOURCE_FILES})
set_target_properties(ember-static
        PROPERTIES
            POSITION_INDEPENDENT_CODE    ON
            C_EXTENSIONS                 OFF
            CXX_EXTENSIONS               OFF
    )
target_link_libraries(ember-static
        PUBLIC
            libember::ember-headers
    )
enable_warnings_on_target(ember-static)

# Alias ember-static to libember::ember-static so that this library can be
# used in lieu of a module from the local source tree
add_library(${PROJECT_NAME}::ember-static ALIAS ember-static)


# Add the IPO property for all relevant targets, if we are building in the
# release configuration and the platform supports it.
if (NOT CMAKE_BUILD_TYPE MATCHES "Debug")
    if (NOT DEFINED check_ipo_supported)
        include(CheckIPOSupported)
        check_ipo_supported(RESULT ipo_supported)
    endif()

    if(ipo_supported)
        set_target_properties(ember-shared PROPERTIES INTERPROCEDURAL_OPTIMIZATION ON)
    endif()
endif()

# <<<  Testing  >>>

# Only enable testing if this is the toplevel cmake file
if (IS_TOPLEVEL)
    add_subdirectory(Tests)
endif()


# <<<  Install  >>>

install(TARGETS ember-headers ember-headeronly ember-shared ember-static EXPORT ${PROJECT_NAME}-targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
install(DIRECTORY Headers/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING
            PATTERN "*.?pp"
    )

# <<<  Export Config  >>>

include(CMakePackageConfigHelpers)

set(EMBER_CMAKE_CONFIG_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

# This makes the project importable from the install directory
install(EXPORT ${PROJECT_NAME}-targets
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${EMBER_CMAKE_CONFIG_DESTINATION}
    )

# Generate the config file and put it into the build directory.
configure_package_config_file(
        ${CMAKE_CURRENT_LIST_DIR}/cmake/${PROJECT_NAME}-config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
        INSTALL_DESTINATION ${EMBER_CMAKE_CONFIG_DESTINATION}
    )

# Generate the version file and put it into the build directory.
write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )

# Install the generated config and version files.
install(
        FILES
            ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
            ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
            DESTINATION ${EMBER_CMAKE_CONFIG_DESTINATION}
    )

