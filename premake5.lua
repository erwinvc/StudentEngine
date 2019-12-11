workspace "StudentEngine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

flags{"MultiProcessorCompile"}

project "StudentEngine"
	location "StudentEngine"
	language "C++"
	staticruntime "off"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"%{prj.name}/**.h",
		"%{prj.name}/**.cpp",
		"%{prj.name}/**.vert",
		"%{prj.name}/**.frag",
		"%{prj.name}/**.tese",
		"%{prj.name}/**.tesc"
	}

	filter { "files:StudentEngine/vendor/**.cpp" }
		flags "NoPCH"	
	filter {}
	
	pchheader "stdafx.h"
	pchsource "StudentEngine/src/stdafx.cpp"
	
	includedirs
	{
		"%{prj.name}/vendor/AssImp/include",
		"%{prj.name}/vendor/AssImp/build/include",
		"%{prj.name}/vendor/GLFW/include",
		"%{prj.name}/vendor/GLEW/include",
		"%{prj.name}/vendor/imgui",
		"%{prj.name}/vendor/OpenAL/include",
		"%{prj.name}",
		"%{prj.name}/src"
	}
	
	libdirs  
	{ 
		"$(SolutionDir)%{prj.name}/vendor/GLFW/lib-vc2015",
		"$(SolutionDir)%{prj.name}/vendor/AssImp",
		"$(SolutionDir)%{prj.name}/vendor/GLEW/lib/Release/x64",
		"$(SolutionDir)%{prj.name}/vendor/OpenAL/libs/Win64",
		"$(SolutionDir)%{prj.name}/vendor/"
	}

	links
	{
		"glfw3",
		"glew32s",
		"opengl32",
		"OpenAL32",
		"IrrXML",
		"zlib.lib",
		"zlibstatic",
		"assimp-vc140-mt"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"GLEW_STATIC",
			"_CRT_SECURE_NO_WARNINGS"
		}


	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"
		defines "DEBUG"
		kind "WindowedApp"

	filter "configurations:Release"
		flags{"LinkTimeOptimization"}
		runtime "Release"
		optimize "On"
		kind "ConsoleApp"