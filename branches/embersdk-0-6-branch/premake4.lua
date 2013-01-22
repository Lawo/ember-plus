solution "EmBER SDK"
    configurations { "DebugStatic", "DebugShared", "ReleaseStatic", "ReleaseShared" }
   
    -- Common settings for all configurations of all projects of this solution
    flags { "ExtraWarnings" }

    -- Common settings for all debug configurations of all projects of this solution
    configuration { "Debug*" }
        defines   { "_DEBUG", "DEBUG" }
        flags     { "Symbols" }

    -- Common settings for all release configurations of all projects of this solution
    configuration { "Release*" }
        defines   { "NDEBUG" }
        flags     { "OptimizeSpeed" }

    -- Common settings for all shared library configurations of all projects of this solution
    configuration { "*Shared" }
        defines   { "LIBEMBER_DLL" }


    -- Disable Visual Studio security warnings
    configuration { "vs*" }
        defines   { "_CRT_SECURE_NO_DEPRECATE" }


    configuration { "DebugStatic" }
        targetdir "bin/debug_static"
         
    configuration { "DebugShared" }
        targetdir "bin/debug_shared"

    configuration { "ReleaseStatic" }
        targetdir "bin/release_static"

    configuration { "ReleaseShared" }
        targetdir "bin/release_shared"
               
    if _ACTION == "clean" then
        os.rmdir("bin")
        os.rmdir("doc")
    end

    -- Define a new action to build the doxygen documentation on request
    newaction {
        trigger     = "doc",
        description = "Create or update the Doxygen documentation for all relevant projects",
        execute = function ()
            os.mkdir("doc")
            os.execute("cd libember && doxygen Doxyfile.in")
            os.execute("cd libs101  && doxygen Doxyfile.in")
        end
    }

    project "ember"
        -- Common settings for all configurations of this project
        location    "libember"
        language    "C++"
        files       { "libember/Headers/**.hpp", "libember/Source/**.cpp", "libember/Headers/**.ipp" }
        includedirs { "libember/Headers" }
            
        -- Common settings for all static library configurations of this project
        configuration { "*Static" }
            kind      "StaticLib"

        -- Common settings for all shared library configurations of this project
        configuration { "*Shared" }
            kind      "SharedLib"
            defines   { "LIBEMBER_DLL_EXPORTS" }


    project "StreamBuffer Test"
        -- Common settings for all configurations of this project
        location    "libember"
        language    "C++"
        kind        "ConsoleApp"
        files       { "libember/Tests/util/StreamBuffer.cpp" }
        includedirs { "libember/Headers" }


    project "Static BER Codec Sample"
        -- Common settings for all configurations of this project
        location    "libember"
        language    "C++"
        kind        "ConsoleApp"
        files       { "libember/Tests/ber/StaticEncodeDecode.cpp" }
        includedirs { "libember/Headers" }
        links       { "ember" } 


    project "Dynamic BER Codec Sample"
        -- Common settings for all configurations of this project
        location    "libember"
        language    "C++"
        kind        "ConsoleApp"
        files       { "libember/Tests/ber/DynamicEncodeDecode.cpp" }
        includedirs { "libember/Headers" }
        links       { "ember" } 


