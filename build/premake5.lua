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
    
    filter "configurations:Debug"
    defines { "_DEBUG" }
    symbols "On"
    filter "configurations:Release"
    defines { "NDEBUG" }
    optimize "On"
    filter { }
    
    
    if os.is("windows") then
    end
    if os.is("linux") then
        buildoptions { "-fPIC -static-libgcc -static-libstdc++" }
        linkoptions { "-Wl,--start-group -lPhysxWrap -lPhysX3_x64 -lPhysX3Cooking_x64 -lPhysX3CharacterKinematic_x64 -lSimulationController -lSceneQuery -lPxTask -lPxPvdSDK_x64 -lPxFoundation_x64 -lLowLevelParticles -lLowLevelDynamics -lLowLevelCloth -lLowLevelAABB -lLowLevel -lPhysX3Common_x64 -lPsFastXml -lPhysX3Extensions -Wl,--end-group" }
        links{
            "pthread",
            "dl",
        }
    end
    
    
project "PhysxWrap"
    kind "StaticLib"
    targetname "PhysxWrap"
    files {
        "../src/physx_wrap/**.h",
        "../src/physx_wrap/**.c",
        "../src/physx_wrap/**.hpp",
        "../src/physx_wrap/**.cpp",
    }
    
    
project "PhysxWrapGo"
    kind "SharedLib"
    targetname "PhysxWrapGo"
    dependson { "PhysxWrap" }
    files {
        "../src/go/**.h",
        "../src/go/**.cpp",
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
    
