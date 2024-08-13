project "TaskHub"
   kind "StaticLib"
   language "C++"
   cppdialect "C++20"
   targetdir "Binaries/%{cfg.buildcfg}"
   staticruntime "on"

   files { "src/**.h", "src/**.cpp" }

   includedirs
   {
        "src",

        "../Vendor/imgui",
        "../Vendor/glfw/include",
        "../Vendor/Glad/include",
        "../Vendor/spdlog/include",
        "../Vendor/miniaudio",
        "../Vendor/stb_image"
   }

   links
   {
        "ImGui",
        "GLFW",
        "Glad"
   }

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

   filter "system:windows"
       systemversion "latest"
       defines { "HUB_PLATFORM_WINDOWS" }

   filter "configurations:Debug"
       defines { "HUB_DEBUG" }
       runtime "Debug"
       symbols "On"

   filter "configurations:Release"
       defines { "HUB_RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "On"

   filter "configurations:Dist"
       defines { "HUB_DIST" }
       runtime "Release"
       optimize "On"
       symbols "Off"