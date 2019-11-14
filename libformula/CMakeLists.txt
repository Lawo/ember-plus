
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

project(libformula VERSION 1.8.2 LANGUAGES CXX)

# Use GNUInstallDirs to make sure libraries are installed into correct locations
# on all platforms.
include(GNUInstallDirs)

################################### Options ####################################


################################# Main Project #################################

# <<<  Build  >>>

add_library(formula INTERFACE)
target_compile_features(formula
        INTERFACE
            cxx_std_98
    )
target_include_directories(formula 
        INTERFACE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Headers>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )

# Alias formula to libformula::formula so that this library can be used
# in lieu of a module from the local source tree
add_library(${PROJECT_NAME}::formula ALIAS formula)


# <<<  Install  >>>

install(TARGETS formula EXPORT ${PROJECT_NAME}-targets
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

set(FORMULA_CMAKE_CONFIG_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

# This makes the project importable from the install directory
install(EXPORT ${PROJECT_NAME}-targets
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${FORMULA_CMAKE_CONFIG_DESTINATION}
    )

# Generate the config file and put it into the build directory.
configure_package_config_file(
        ${CMAKE_CURRENT_LIST_DIR}/cmake/${PROJECT_NAME}-config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
        INSTALL_DESTINATION ${FORMULA_CMAKE_CONFIG_DESTINATION}
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
        DESTINATION ${FORMULA_CMAKE_CONFIG_DESTINATION}
    )

