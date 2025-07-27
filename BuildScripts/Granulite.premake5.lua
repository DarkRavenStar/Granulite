dofile("Helper.lua")

--include "GLFW.premake5.lua"
--include "Volk.premake5.lua"
--include "Vulkan.premake5.lua"

project "Granulite"
	kind "ConsoleApp"
	language "C++"
	location(projDir)
	targetdir (targetDirMain)
	objdir (objDirMain)

	local granuliteIncludeDir = mainDir .. "/Source"

	-- Include the files in the project
	addFilesWithFilters
	(
		{
			granuliteIncludeDir .. "/**.cpp",
			granuliteIncludeDir .. "/**.h",
		},
		{
			granuliteIncludeDir .. "/TestMain.cpp",
		}
	)

	includedirs
	{
		granuliteIncludeDir,
		glfwIncludeDir,
		vulkanIncludeDir,
		volkIncludeDir,
		imguiIncludeDir,
		vkBootstrapIncludeDir,
	}

	libdirs
	{
		glfwLibDir,
		vulkanLibDir,
	}

	links
	{
		glfwLibName,
		vulkanLibName,
		volkLibName,
		imguiLibName,
		vkBootstrapLibName,
		--"user32",
		--"gdi32",
		--"opengl32",
	}

	defines
	{
		--"IMGUI_IMPL_VULKAN_USE_VOLK",
		--"VOLK_IMPLEMENTATION",
		"VK_USE_PLATFORM_WIN32_KHR",
		"VK_NO_PROTOTYPES",
		"GLFW_INCLUDE_NONE",
		"GLFW_INCLUDE_VULKAN"
	}

	dependson { "ZeroCheck" }