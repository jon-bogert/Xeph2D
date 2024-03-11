workspace "Xeph2D"
	architecture "x64"
	configurations
	{
		"Debug",
		"Debug (Console)",
		"Editor",
		"Editor (Console)",
		"Release"
	}
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- ==============================

project "Xeph2D"
	location "%{prj.name}"
	kind "StaticLib"
	language "C++"
	targetname "%{prj.name}"
	targetdir ("bin/".. outputdir)
	objdir ("%{prj.name}/int/".. outputdir)
	cppdialect "C++17"
	staticruntime "Off"
	
	defines { "SFML_STATIC", "YAML_CPP_STATIC_DEFINE" }

	files
	{
		"%{prj.name}/**.h",
		"%{prj.name}/**.cpp",
	}
	
	includedirs
	{
		"%{prj.name}/include",
		"ext/box2d/include",
		"ext/yaml-cpp/include",
		"ext/SFML/include",
		"ext/xe-markup/include",
		"ext/yaml-cpp/include"
	}
	
	filter "system:windows"
		systemversion "latest"
		defines { "WIN32" }

	filter "configurations:Debug"
		defines { "_DEBUG", "NCONSOLE" }
		symbols "On"

	filter "configurations:Debug (Console)"
		defines { "_DEBUG", "_CONSOLE" }
		symbols "On"
		
	filter "configurations:Editor"
		defines { "_DEBUG", "NCONSOLE", "_EDITOR" }
		symbols "On"
		includedirs { "ext/ImGui/include" }

	filter "configurations:Editor (Console)"
		defines { "_DEBUG", "_CONSOLE", "_EDITOR" }
		symbols "On"
		includedirs { "ext/ImGui/include" }
		
	filter "configurations:Release"
		defines { "NDEBUG", "NCONSOLE" }
		optimize "On"
		
-- ==============================

project "App"
	location "%{prj.name}"
	kind "WindowedApp"
	language "C++"
	targetname "%{prj.name}"
	targetdir ("bin/".. outputdir)
	objdir ("%{prj.name}/int/".. outputdir)
	cppdialect "C++17"
	staticruntime "Off"
	
	defines { "SFML_STATIC", "YAML_CPP_STATIC_DEFINE" }

	files
	{
		"%{prj.name}/**.h",
		"%{prj.name}/**.cpp",
	}
	
	includedirs
	{
		"%{prj.name}/Assets/Components",
		"ext/box2d/include",
		"ext/SFML/include",
		"ext/yaml-cpp/include",
		"Xeph2D/include",
		"%{prj.name}/gen",
		"ext/xe-markup/include",
		"ext/yaml-cpp/include"
	}
	
	libdirs
	{
		"%{prj.name}/lib"
	}
	
	links
	{
		"Xeph2D",
		"opengl32",
		"winmm",
		"gdi32",
		"Shell32",
		"flac",
		"freetype",
		"ogg",
		"openal32",
		"vorbis",
		"vorbisenc.lib",
		"vorbisfile"
	}
	
	filter "system:windows"
		systemversion "latest"
		defines { "WIN32" }

	filter "configurations:Debug"
		defines { "_DEBUG", "NCONSOLE" }
		symbols "On"
		links
		{
			"box2d-d.lib",
			"sfml-system-s-d.lib",
			"sfml-graphics-s-d.lib",
			"sfml-window-s-d.lib",
			"sfml-audio-s-d.lib",
			"sfml-network-s-d.lib",
			"xe-markup-d.lib",
			"yaml-cpp-d.lib"
		}

	filter "configurations:Debug (Console)"
		defines { "_DEBUG", "_CONSOLE" }
		kind "ConsoleApp"
		symbols "On"
		links
		{
			"box2d-d.lib",
			"sfml-system-s-d.lib",
			"sfml-graphics-s-d.lib",
			"sfml-window-s-d.lib",
			"sfml-audio-s-d.lib",
			"sfml-network-s-d.lib",
			"xe-markup-d.lib",
			"yaml-cpp-d.lib"
		}
		
	filter "configurations:Editor"
		defines { "_DEBUG", "NCONSOLE", "_EDITOR" }
		symbols "On"
		includedirs { "ext/ImGui/include" }
		links
		{
			"box2d-d.lib",
			"sfml-system-s-d.lib",
			"sfml-graphics-s-d.lib",
			"sfml-window-s-d.lib",
			"sfml-audio-s-d.lib",
			"sfml-network-s-d.lib",
			"imgui-sfml-s-d.lib",
			"opengl32",
			"xe-markup-d.lib",
			"yaml-cpp-d.lib"
		}

	filter "configurations:Editor (Console)"
		defines { "_DEBUG", "_CONSOLE", "_EDITOR" }
		kind "ConsoleApp"
		symbols "On"
		includedirs { "ext/ImGui/include" }
		links
		{
			"box2d-d.lib",
			"sfml-system-s-d.lib",
			"sfml-graphics-s-d.lib",
			"sfml-window-s-d.lib",
			"sfml-audio-s-d.lib",
			"sfml-network-s-d.lib",
			"imgui-sfml-s-d.lib",
			"xe-markup-d.lib",
			"yaml-cpp-d.lib"
		}
		
	filter "configurations:Release"
		defines { "NDEBUG", "NCONSOLE" }
		optimize "On"
		links
		{
			"box2d.lib",
			"sfml-system-s.lib",
			"sfml-graphics-s.lib",
			"sfml-window-s.lib",
			"sfml-audio-s.lib",
			"sfml-network-s.lib",
			"xe-markup.lib",
			"yaml-cpp.lib"
		}