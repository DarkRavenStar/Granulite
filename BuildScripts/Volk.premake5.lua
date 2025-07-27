--include "Vulkan.premake5.lua"

local dir = mainDir .. "/ThirdParty/volk"
volkIncludeDir = dir
volkLibName = "Volk"

project "Volk"
	kind "StaticLib"
	language "C"
	location(projDir)
	targetdir (targetDirMain)
	objdir (objDirMain)
	
	files
	{
        dir .. "/volk.h",
        dir .. "/volk.c",
    }

	includedirs
	{
		vulkanIncludeDir
	}

	libdirs
	{
		vulkanLibDir
	}

	links
	{
		vulkanLibName
	}

	defines
	{
		--"VOLK_IMPLEMENTATION",
      	"VK_USE_PLATFORM_WIN32_KHR",
		"VK_NO_PROTOTYPES",
	}