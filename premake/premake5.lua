workspace "SkylandersEditor"
   configurations { "Debug", "Release" }
   platforms { "Win32", "Win64"}

   location "../"

   targetdir "../bin/%{cfg.buildcfg}"

   includedirs {"../hidapi/hidapi"}

   filter "configurations:Debug"
      defines { "DEBUG", "TRACE" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

	filter { "platforms:Win32" }
	  system "Windows"
	  architecture "x86"
  
  	filter { "platforms:Win64" }
	  system "Windows"
	  architecture "x86_64"

project "hidapi"
	kind "StaticLib"
	language "C"

	location "../hidapi"

	files {"../hidapi/hidapi/hidapi.h", "../hidapi/windows/hid.c", "../hidapi/windows/hidapi_winapi.h"}
		

project "SkylandersEditor"
   kind "ConsoleApp"
   language "C++"
   location "../SkylandersEditor"   

   links "hidapi"

   files { "../SkylandersEditor/include/**.h", "../SkylandersEditor/src/**.cpp" }
   includedirs {"../SkylandersEditor/include"}