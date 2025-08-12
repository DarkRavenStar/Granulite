local dir = mainDir .. "/ThirdParty/VMA"
vmaIncludeDir = dir .. "/include"
local vmaSourceDir = dir .. "/src"

vmaLibName = "VMA"

project "VMA"
	--kind "StaticLib"
	kind "Utility" -- prevents it from producing a binary
    staticruntime "off"
	language "C++"
	location(projDir)
	targetdir (targetDirMain)
	objdir (objDirMain)
	--cppdialect "c++20"
	
	files
	{
        vmaIncludeDir .. "/**h",
        vmaSourceDir .. "/**.*"
    }
--[[

	includedirs
	{
		volkIncludeDir,
		vulkanIncludeDir,
	}
	
	libdirs
	{
		volkLibDir,
		vulkanLibDir,
	}
	
	links
	{
		volkLibName,
		vulkanLibName,
	}
	
	defines
	{
		"VOLK_HEADER_VERSION",
		"VMA_DYNAMIC_VULKAN_FUNCTIONS=0",
		"VMA_STATIC_VULKAN_FUNCTIONS=0",
		"VK_NO_PROTOTYPES",
	}
	]]
	
	filter { "files:**.natvis" }
	buildaction "None"     -- don't compile it
	--deploymode "Copy"      -- ensures it gets into the output dir (optional)
	
	filter { "files:" .. path.getrelative(os.getcwd(), vmaSourceDir) .. "/**.*" }
		flags { "ExcludeFromBuild" }

	filter {} -- Clear filter