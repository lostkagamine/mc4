workspace "mc4"
    configurations { "Debug", "Release" }

    language "C"
    -- Write generates files here
    location "bld"
    -- Put compiled stuff here
    targetdir "bld/%{cfg.buildcfg}"

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

project "libmc4"
    -- Avoid 'liblib' syndrome on Linux
    targetname "mc4"
    kind "StaticLib"

    files { "libmc4/**.h", "libmc4/**.c" }

project "mc4-cli"
    kind "ConsoleApp"

    files { "mc4-cli/mc4cli.c" }
    links { "libmc4" }
