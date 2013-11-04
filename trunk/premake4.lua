solution "EmberPlus SDK"
    configurations { "ReleaseShared", "ReleaseStatic", "DebugShared", "DebugStatic" }
   
    -- Common settings for all configurations of all projects of this solution
    location "build"
    flags { "ExtraWarnings" }

    -- Common settings for all debug configurations of all projects of this solution
    configuration { "Debug*" }
        defines   { "_DEBUG", "DEBUG" }
        flags     { "Symbols" }

    -- Common settings for all release configurations of all projects of this solution
    configuration { "Release*" }
        defines   { "NDEBUG" }
        flags     { "OptimizeSpeed", "FloatFast" }

    -- Common settings for all shared library configurations of all projects of this solution
    configuration { "*Shared" }
        defines   { "LIBEMBER_DLL" }


    -- Disable Visual Studio security warnings
    configuration { "vs*" }
        defines   { "_CRT_SECURE_NO_DEPRECATE" }

    -- Change GCC behaviour to only export explicitly marked symbols and set LDs rpath to the
    -- same directory as the one a binary is in
    configuration { "gmake and not macosx" }
        buildoptions { "-fvisibility=hidden", "-fvisibility-inlines-hidden" }
        linkoptions { "-Wl,-rpath=." }


    configuration { "DebugStatic" }
        targetdir "build/debug/static/bin"
        objdir    "build/debug/static/obj"
        libdirs   { "build/debug/static/bin" }
         
    configuration { "DebugShared" }
        targetdir "build/debug/shared/bin"
        objdir    "build/debug/shared/obj"
        libdirs   { "build/debug/shared/bin" }

    configuration { "ReleaseStatic" }
        targetdir "build/release/static/bin"
        objdir    "build/release/static/obj"
        libdirs   { "build/release/static/bin" }

    configuration { "ReleaseShared" }
        targetdir "build/release/shared/bin"
        objdir    "build/release/shared/obj"
        libdirs   { "build/release/shared/bin" }
               
    if _ACTION == "clean" then
        os.rmdir("build")
        os.rmdir("doc")
    end

    -- Define a new action to build the doxygen documentation on request
    newaction {
        trigger     = "doc",
        description = "Create or update the Doxygen documentation for all relevant projects",
        execute = function ()
            os.mkdir("build/doc")
            os.execute("cd libember && doxygen Doxyfile.in")
            os.execute("cd libs101  && doxygen Doxyfile.in")
        end
    }

    project "EmberPlus C++ Library"
        -- Common settings for all configurations of this project
        language    "C++"
        files       { "libember/Headers/**.hpp", "libember/Source/**.cpp", "libember/Headers/**.ipp" }
        includedirs { "libember/Headers" }

        configuration { "windows" }
            targetname  "libember"

        configuration { "not windows" }
            targetname  "ember"
            
        configuration { "gmake or not windows" }
            buildoptions { "-std=c++03" }

        configuration { "macosx" }
            buildoptions { "-stdlib=libc++" }
            linkoptions { "-stdlib=libc++" }

        -- Common settings for all static library configurations of this project
        configuration { "*Static" }
            kind      "StaticLib"

        -- Common settings for all shared library configurations of this project
        configuration { "*Shared" }
            kind      "SharedLib"
            defines   { "LIBEMBER_DLL_EXPORTS" }

    project "EmberPlus Library Test - StreamBuffer"
        -- Common settings for all configurations of this project
        language    "C++"
        kind        "ConsoleApp"
        targetname   "test-libemeber-streambuffer"
        files       { "libember/Tests/util/StreamBuffer.cpp" }
        includedirs { "libember/Headers" }

    project "EmberPlus Library Test - GlowValue"
        -- Common settings for all configurations of this project
        language    "C++"
        kind        "ConsoleApp"
        targetname   "test-libemeber-glowvalue"
        files       { "libember/Tests/glow/GlowValue.cpp" }
        includedirs { "libember/Headers" }
        links       { "EmberPlus C++ Library" } 

    project "EmberPlus Library Sample - Static BER Codec"
        -- Common settings for all configurations of this project
        language    "C++"
        kind        "ConsoleApp"
        targetname  "sample-libember-staticbercodec"
        files       { "libember/Tests/ber/StaticEncodeDecode.cpp" }
        includedirs { "libember/Headers" }
        links       { "EmberPlus C++ Library" } 

    project "EmberPlus Library Sample - Dynamic BER Codec"
        -- Common settings for all configurations of this project
        language    "C++"
        kind        "ConsoleApp"
        targetname  "sample-libember-dynamicbercodec"
        files       { "libember/Tests/ber/DynamicEncodeDecode.cpp" }
        includedirs { "libember/Headers" }
        links       { "EmberPlus C++ Library" } 


    project "EmberPlus C Library"
        -- Common settings for all configurations of this project
        language    "C"
        files       { "libember_slim/Source/**.h", "libember_slim/Source/**.c" }
        excludes    { "**__sample*" }
        includedirs { "libember_slim/Source" }
            
        configuration { "windows" }
            targetname  "libember_slim"

        configuration { "not windows" }
            targetname  "ember_slim"
            
        -- Common settings for all static library configurations of this project
        configuration { "*Static" }
            kind      "StaticLib"

        -- Common settings for all shared library configurations of this project
        configuration { "*Shared" }
            kind      "SharedLib"
            defines   { "LIBEMBER_DLL_EXPORTS" }

