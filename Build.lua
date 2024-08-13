-- premake5.lua
workspace "TaskHub"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "TaskHubApp"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"


group "TaskHub"
   include "TaskHub/Build-TaskHub.lua"
group ""

group "Dependencies"
   include "Vendor/imgui"
   include "Vendor/glfw"
   include "Vendor/Glad"
   include "Vendor/miniaudio"
group ""

include "TaskHubApp/Build-App.lua"