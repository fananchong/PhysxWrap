workspace "PhysxWrap"
    configurations { "Debug", "Release" }
    platforms { "x64" }
    targetdir "../bin/%{cfg.buildcfg}"
    language "C++"
    flags {
        "C++11",
        "StaticRuntime",
    }
    includedirs {
        "../deps/include/",
        "../deps/include/physx/PhysX_3.4",
        "../deps/include/physx/PxShared",
    }
    libdirs {
        "../deps/lib_win",
        "../deps/lib_linux",
    }
    filter "configurations:Debug"
        libdirs{
            "../bin/Debug",
        }
    filter "configurations:Release"
        libdirs{
            "../bin/Release",
        }
    filter { }
    
    defines {
        "EXPORT_DLL"
    }
    
    filter "configurations:Debug"
    defines { "_DEBUG" }
    symbols "On"
    filter "configurations:Release"
    defines { "NDEBUG" }
    optimize "On"
    filter { }
    
    
    if os.is("windows") then
        includedirs {
            "../deps/include/python/win",
        }
    end
    if os.is("linux") then
    
    end
    
    
project "PhysxWrap"
    kind "SharedLib"
    targetname "PhysxWrap"
    files {
        "../src/physx_wrap/**.h",
        "../src/physx_wrap/**.c",
        "../src/physx_wrap/**.hpp",
        "../src/physx_wrap/**.cpp",
    }
        
project "test"
    kind "ConsoleApp"
    targetname "test"
    dependson { "PhysxWrap" }
    includedirs {
        "../src/physx_wrap/",
    }
    files {
        "../src/test/*.h",
        "../src/test/*.cpp",
    }
    