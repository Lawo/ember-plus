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

    -- Change GCC behaviour to mimic the symbol export behavior
    configuration { "gmake or not windows" }
        buildoptions { "-fvisibility=hidden", "-fvisibility-inlines-hidden" }


    configuration { "DebugStatic" }
        targetdir "build/debug/static/bin"
        objdir    "build/debug/static/obj"
         
    configuration { "DebugShared" }
        targetdir "build/debug/shared/bin"
        objdir    "build/debug/shared/obj"

    configuration { "ReleaseStatic" }
        targetdir "build/release/static/bin"
        objdir    "build/release/static/obj"

    configuration { "ReleaseShared" }
        targetdir "build/release/shared/bin"
        objdir    "build/release/shared/obj"
               
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
        targetname  "libember"
        files       { "libember/Headers/**.hpp", "libember/Source/**.cpp", "libember/Headers/**.ipp" }
        includedirs { "libember/Headers" }
            
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


    project "EmberPlus Library Sample - Static BER Codec"
        -- Common settings for all configurations of this project
        language    "C++"
        kind        "ConsoleApp"
        targetname  "sample-libember-staticbercodec"
        files       { "libember/Tests/ber/StaticEncodeDecode.cpp" }
        includedirs { "libember/Headers" }
        links       { "ember" } 

    project "EmberPlus Library Sample - Dynamic BER Codec"
        -- Common settings for all configurations of this project
        language    "C++"
        kind        "ConsoleApp"
        targetname  "sample-libember-dynamicbercodec"
        files       { "libember/Tests/ber/DynamicEncodeDecode.cpp" }
        includedirs { "libember/Headers" }
        links       { "ember" } 


    project "EmberPlus C Library"
        -- Common settings for all configurations of this project
        language    "C"
        targetname  "libember_slim"
        files       {
            "libember_slim/Source/**.h",
            "libember_slim/Source/ber.c", "libember_slim/Source/berio.c", "libember_slim/Source/berreader.c", "libember_slim/Source/bertag.c",
            "libember_slim/Source/bytebuffer.c", "libember_slim/Source/ember.c", "libember_slim/Source/emberasyncreader.c", "libember_slim/Source/emberframing.c",
            "libember_slim/Source/emberglow.c", "libember_slim/Source/emberinternal.c" }
        includedirs { "libember_slim/Source" }
            
        -- Common settings for all static library configurations of this project
        configuration { "*Static" }
            kind      "StaticLib"

        -- Common settings for all shared library configurations of this project
        configuration { "*Shared" }
            kind      "SharedLib"
            defines   { "LIBEMBER_DLL_EXPORTS" }

