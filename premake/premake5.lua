workspace "SkylandersEditor"
	
	configurations {"Debug", "Release"}
	platforms {"x64"}

	defaultplatform "x64"

	location "../"

	targetdir "../bin/%{cfg.buildcfg}"

	startproject "SkulandersEditor"

project "raylib"

	kind "StaticLib"
		
	filter "action:vs*"
		defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS", "_WIN32"}
		links {"winmm"}
				
	filter{}
	
	location "../projects/raylib/"
	language "C"
	targetdir "../bin/%{cfg.buildcfg}"
	
	includedirs { "../externals/raylib/src", "../externals/raylib/src/external/glfw/include"}
	vpaths 
	{
		["Header Files"] = { "../externals/raylib/src/**.h"},
		["Source Files/*"] = {"../externals/raylib/src/**.c"},
	}
	files {"../externals/raylib/src/*.h", "../externals/raylib/src/*.c"}
	
	defines{"PLATFORM_DESKTOP", "GRAPHICS_API_OPENGL_33"}

project "raygui"
	kind "StaticLib"
	language "C"

	location "../projects/raygui"

	includedirs {"../externals/raygui/src", "../externals/raylib/src"}

	files "../externals/raygui/src/**.h"

project "hidapi"
	kind "StaticLib"
	language "C"

	location "../projects/hidapi"

	includedirs {"../externals/hidapi/hidapi/"}
	files {"../externals/hidapi/hidapi/hidapi.h", "../externals/hidapi/windows/hid.c", "../externals/hidapi/windows/hidapi_winapi.h"}

project "SkylandersEditor"
	kind "ConsoleApp"

	language "C++"

	location "../projects/SkylandersEditor"

	intrinsics "On"

	vpaths
	{
		["Source Files/*"] = "../SkylandersEditor/src/**.cpp",
		["Header Files/*"] = "../SkylandersEditor/include/**.h",
	}

	links {"raylib", "hidapi"}

	includedirs
	{
		"../externals/raylib/src",
		"../externals/hidapi/hidapi",
		"../externals/raygui/src",
		"../SkylandersEditor/include"
	}

	files {
		"../SkylandersEditor/src/**.cpp",
		"../SkylandersEditor/include/**.h"
	}

	filter "action:vs*"
		defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS", "_WIN32"}
		dependson {"raylib", "hidapi", "raygui"}
		links {"raylib.lib", "hidapi.lib"}
        characterset ("MBCS")

	filter "system:windows"
		defines{"_WIN32"}
		links {"winmm", "kernel32", "opengl32", "kernel32", "gdi32"}
		libdirs {"../bin/%{cfg.buildcfg}"}