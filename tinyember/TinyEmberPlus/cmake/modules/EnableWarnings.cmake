if (NOT ENABLE_WARNINGS_INCLUDED)
    set(ENABLE_WARNINGSS_INCLUDED 1)

    function(enable_warnings_on_target target)
        if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU")
            target_compile_options(${target} PRIVATE -Wall -Wextra -Wunreachable-code -Wpedantic -Wno-long-long)
        endif()

        if ( CMAKE_CXX_COMPILER_ID MATCHES "MSVC" )
            string(REGEX REPLACE "/W[0-9]" "/W4" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}) # override default warning level
            target_compile_options(${target} PRIVATE /w44265 /w44061 /w44062 )
        endif()
    endfunction(enable_warnings_on_target)
endif()

