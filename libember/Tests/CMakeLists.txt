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


add_executable(libember-test-decode_length_check ber/DecodeLengthCheck.cpp)
set_target_properties(libember-test-decode_length_check
        PROPERTIES
            POSITION_INDEPENDENT_CODE    ON
            VISIBILITY_INLINES_HIDDEN    ON
            C_VISIBILITY_PRESET          hidden
            CXX_VISIBILITY_PRESET        hidden
            C_EXTENSIONS                 OFF
            CXX_EXTENSIONS               OFF
    )
target_link_libraries(libember-test-decode_length_check PRIVATE ember-headeronly)
enable_warnings_on_target(libember-test-decode_length_check)


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
        set_target_properties(libember-test-decode_length_check   PROPERTIES INTERPROCEDURAL_OPTIMIZATION ON)
        set_target_properties(libember-test-glow_value            PROPERTIES INTERPROCEDURAL_OPTIMIZATION ON)
    endif()
endif()


include(CTest)

add_test(NAME length-negative_zero COMMAND libember-test-decode_length_check negative_zero)
add_test(NAME length-encoded_length COMMAND libember-test-decode_length_check encoded_length)
add_test(NAME length-exponent_length COMMAND libember-test-decode_length_check exponent_length)
add_test(NAME length-zero_mantissa COMMAND libember-test-decode_length_check zero_mantissa)
add_test(NAME length-preamble_length_not_one COMMAND libember-test-decode_length_check preamble_length_not_one)
add_test(NAME length-integral COMMAND libember-test-decode_length_check integral)
add_test(NAME length-boolean COMMAND libember-test-decode_length_check boolean)
add_test(NAME length-length_first_byte COMMAND libember-test-decode_length_check length_first_byte)
add_test(NAME length-length_second_byte COMMAND libember-test-decode_length_check length_second_byte)
add_test(NAME length-object_identifier_empty COMMAND libember-test-decode_length_check object_identifier_empty)
add_test(NAME length-object_identifier_too_short COMMAND libember-test-decode_length_check object_identifier_too_short)
add_test(NAME length-tag COMMAND libember-test-decode_length_check tag)
add_test(NAME length-tag_multibyte COMMAND libember-test-decode_length_check tag_multibyte)
add_test(NAME length-tag_multibyte_too_short COMMAND libember-test-decode_length_check tag_multibyte_too_short)
