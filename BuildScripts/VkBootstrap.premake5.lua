--include "Vulkan.premake5.lua"

local dir = mainDir .. "/ThirdParty/vk-bootstrap/src"
vkBootstrapIncludeDir = dir
vkBootstrapLibName = "VkBootstrap"



project "VkBootstrap"
	kind "StaticLib"
	language "C++"
	location(projDir)
	targetdir (targetDirMain)
	objdir (objDirMain)
	
	files
	{
        dir .. "/**.h",
        dir .. "/**.cpp",
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
      	"VK_USE_PLATFORM_WIN32_KHR",
		"VK_NO_PROTOTYPES",
	}