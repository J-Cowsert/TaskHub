-- premake5.lua
workspace "TaskHub"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "TaskHubApp"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

include "Build-External.lua"
include "TaskHubApp/Build-App.lua"