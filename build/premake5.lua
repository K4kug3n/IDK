workspace "Idk"
   configurations { "Debug", "Release" }
   platforms { "Win32", "Win64", "linux86", "linux64", "macos" }

project "Idk"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"
   targetdir "bin/%{cfg.buildcfg}"

   includedirs { "../include" }
   includedirs { "../thirdparty/include" }

   links { "SDL2.lib", "SDL2main.lib" }
   defines { "VK_NO_PROTOTYPES" }

   files { "../**.hpp", "../**.cpp", "../**.inl" }

   filter "configurations:Debug"
      defines { "IDK_DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "IDK_NDEBUG" }
      optimize "On"


   filter { "platforms:Win32" }
      system "Windows"
      architecture "x86"
      libdirs("../thirdparty/lib/x86")

   filter { "platforms:Win64" }
      system "Windows"
      architecture "x86_64"
      libdirs("../thirdparty/lib/x64")

   filter { "system:linux" }
      defines{ "IDK_UNIX", "VK_USE_PLATFORM_XLIB_KHR" }
   filter { "system:windows" }
      defines{ "IDK_WINDOWS", "VK_USE_PLATFORM_WIN32_KHR" }
   filter { "system:macosx" }
      defines{ "IDK_MACOSX" }

   filter { "action:gmake*", "toolset:gcc" }
      links { "dl", "stdc++fs" }