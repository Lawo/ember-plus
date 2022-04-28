project(ember_slim)



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

project(libember_slim VERSION 1.8.3 LANGUAGES C)

# Use GNUInstallDirs to make sure libraries are installed into correct locations
# on all platforms.
include(GNUInstallDirs)

################################### Options ####################################


################################# Main Project #################################

include(cmake/modules/EnableWarnings.cmake)

# <<<  Build  >>>

set(SOURCE_FILES
        Source/ber.c
        Source/berio.c
        Source/berreader.c
        Source/bertag.c
        Source/bytebuffer.c
        Source/ember.c
        Source/emberasyncreader.c
        Source/emberframing.c
        Source/emberinternal.c
        Source/glow.c
        Source/glowrx.c
        Source/glowtx.c
    )


add_library(ember_slim-shared SHARED ${SOURCE_FILES})
set_target_properties(ember_slim-shared
        PROPERTIES
            POSITION_INDEPENDENT_CODE    ON
            VISIBILITY_INLINES_HIDDEN    ON
            C_VISIBILITY_PRESET          hidden
            CXX_VISIBILITY_PRESET        hidden
            C_EXTENSIONS                 OFF
            CXX_EXTENSIONS               OFF
    )
target_compile_definitions(ember_slim-shared
        PUBLIC
            LIBEMBER_DLL
        PRIVATE
            LIBEMBER_DLL_EXPORTS
    ) 
enable_warnings_on_target(ember_slim-shared)

# Alias ember_slim-shared to libember_slim::ember_slim-shared so that this
# library can be used in lieu of a module from the local source tree
add_library(${PROJECT_NAME}::ember_slim-shared ALIAS ember_slim-shared)


add_library(ember_slim-static STATIC ${SOURCE_FILES})
set_target_properties(ember_slim-static
        PROPERTIES
            POSITION_INDEPENDENT_CODE    ON
            C_EXTENSIONS                 OFF
            CXX_EXTENSIONS               OFF
    )
enable_warnings_on_target(ember_slim-static)

# Alias ember_slim-static to libember_slim::ember_slim-static so that this
# library can be used in lieu of a module from the local source tree
add_library(${PROJECT_NAME}::ember_slim-static ALIAS ember_slim-static)


# Add the IPO property for all relevant targets, if we are building in the
# release configuration and the platform supports it.
if (NOT CMAKE_BUILD_TYPE MATCHES "Debug")
    if (NOT DEFINED check_ipo_supported)
        include(CheckIPOSupported)
        check_ipo_supported(RESULT ipo_supported)
    endif()

    if(ipo_supported)
        set_target_properties(ember_slim-shared PROPERTIES INTERPROCEDURAL_OPTIMIZATION ON)
    endif()
endif()


# <<<  Install  >>>

install(TARGETS ember_slim-shared ember_slim-static EXPORT ${PROJECT_NAME}-targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
install(FILES 
            Source/api.h
            Source/ber.h
            Source/berio.h
            Source/berreader.h
            Source/bertag.h
            Source/bertypes.h
            Source/bytebuffer.h
            Source/ember.h
            Source/emberasyncreader.h
            Source/emberframing.h
            Source/emberinternal.h
            Source/emberplus.h
            Source/glow.h
            Source/glowrx.h
            Source/glowtx.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ember_slim
    )

# <<<  Export Config  >>>

include(CMakePackageConfigHelpers)

set(EMBER_SLIM_CMAKE_CONFIG_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

# This makes the project importable from the install directory
install(EXPORT ${PROJECT_NAME}-targets
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${EMBER_SLIM_CMAKE_CONFIG_DESTINATION}
    )

# Generate the config file and put it into the build directory.
configure_package_config_file(
        ${CMAKE_CURRENT_LIST_DIR}/cmake/${PROJECT_NAME}-config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
        INSTALL_DESTINATION ${EMBER_SLIM_CMAKE_CONFIG_DESTINATION}
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
            DESTINATION ${EMBER_SLIM_CMAKE_CONFIG_DESTINATION}
    )

