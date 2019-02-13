include(../cmake/modules/EnableWarnings.cmake)


add_executable(libember-test-streambuffer util/StreamBuffer.cpp)
set_target_properties(libember-test-streambuffer
        PROPERTIES
            POSITION_INDEPENDENT_CODE    ON
            VISIBILITY_INLINES_HIDDEN    ON
            C_VISIBILITY_PRESET          hidden
            CXX_VISIBILITY_PRESET        hidden
            C_EXTENSIONS                 OFF
            CXX_EXTENSIONS               OFF
    )
target_link_libraries(libember-test-streambuffer PRIVATE ember-headeronly)
enable_warnings_on_target(libember-test-streambuffer)


add_executable(libember-test-static_encode_decode ber/StaticEncodeDecode.cpp)
set_target_properties(libember-test-static_encode_decode
        PROPERTIES
            POSITION_INDEPENDENT_CODE    ON
            VISIBILITY_INLINES_HIDDEN    ON
            C_VISIBILITY_PRESET          hidden
            CXX_VISIBILITY_PRESET        hidden
            C_EXTENSIONS                 OFF
            CXX_EXTENSIONS               OFF
    )
target_link_libraries(libember-test-static_encode_decode PRIVATE ember-headeronly)
enable_warnings_on_target(libember-test-static_encode_decode)


add_executable(libember-test-dynamic_encode_decode ber/DynamicEncodeDecode.cpp)
set_target_properties(libember-test-dynamic_encode_decode
        PROPERTIES
            POSITION_INDEPENDENT_CODE    ON
            VISIBILITY_INLINES_HIDDEN    ON
            C_VISIBILITY_PRESET          hidden
            CXX_VISIBILITY_PRESET        hidden
            C_EXTENSIONS                 OFF
            CXX_EXTENSIONS               OFF
    )
target_link_libraries(libember-test-dynamic_encode_decode PRIVATE ember-headeronly)
enable_warnings_on_target(libember-test-dynamic_encode_decode)


add_executable(libember-test-glow_value glow/GlowValue.cpp)
set_target_properties(libember-test-glow_value
        PROPERTIES
            POSITION_INDEPENDENT_CODE    ON
            VISIBILITY_INLINES_HIDDEN    ON
            C_VISIBILITY_PRESET          hidden
            CXX_VISIBILITY_PRESET        hidden
            C_EXTENSIONS                 OFF
            CXX_EXTENSIONS               OFF
    )
target_link_libraries(libember-test-glow_value PRIVATE ember-headeronly)
enable_warnings_on_target(libember-test-glow_value)


# Add the IPO property for all relevant targets, if we are building in the
# release configuration and the platform supports it.
if (NOT CMAKE_BUILD_TYPE MATCHES "Debug")
    if (NOT DEFINED check_ipo_supported)
        include(CheckIPOSupported)
        check_ipo_supported(RESULT ipo_supported)
    endif()

    if(ipo_supported)
        set_target_properties(libember-test-streambuffer          PROPERTIES INTERPROCEDURAL_OPTIMIZATION ON)
        set_target_properties(libember-test-static_encode_decode  PROPERTIES INTERPROCEDURAL_OPTIMIZATION ON)
        set_target_properties(libember-test-dynamic_encode_decode PROPERTIES INTERPROCEDURAL_OPTIMIZATION ON)
        set_target_properties(libember-test-glow_value            PROPERTIES INTERPROCEDURAL_OPTIMIZATION ON)
    endif()
endif()

