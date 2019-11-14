
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

project(libs101 VERSION 1.8.2 LANGUAGES CXX)

# Use GNUInstallDirs to make sure libraries are installed into correct locations
# on all platforms.
include(GNUInstallDirs)

################################### Options ####################################


################################# Main Project #################################

# <<<  Build  >>>

add_library(s101 INTERFACE)
target_compile_features(s101
        INTERFACE
            cxx_std_98
    )
target_include_directories(s101 
        INTERFACE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Headers>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )

# Alias s101 to libs101::s101 so that this library can be used
# in lieu of a module from the local source tree
add_library(${PROJECT_NAME}::s101 ALIAS s101)


# <<<  Install  >>>

install(TARGETS s101 EXPORT ${PROJECT_NAME}-targets
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

set(S101_CMAKE_CONFIG_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

# This makes the project importable from the install directory
install(EXPORT ${PROJECT_NAME}-targets
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${S101_CMAKE_CONFIG_DESTINATION}
    )

# Generate the config file and put it into the build directory.
configure_package_config_file(
        ${CMAKE_CURRENT_LIST_DIR}/cmake/${PROJECT_NAME}-config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
        INSTALL_DESTINATION ${S101_CMAKE_CONFIG_DESTINATION}
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
        DESTINATION ${S101_CMAKE_CONFIG_DESTINATION}
    )

