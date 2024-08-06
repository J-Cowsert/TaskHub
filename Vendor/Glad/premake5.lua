project "Glad"
	kind "StaticLib"
	language "C"
    staticruntime "on"

	targetdir ("bin/" .. OutputDir .. "/%{prj.name}")
	objdir ("bin-int/" .. OutputDir .. "/%{prj.name}")

	files
	{
		"include/glad/glad.h",
        "include/KHR/khrplatform.h",
        "src/glad.c"
	}

    includedirs
    {
        "include"
    }

	filter "system:windows"
        systemversion "latest"