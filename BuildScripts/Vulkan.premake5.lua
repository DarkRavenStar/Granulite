local useEnv_Vulkan = true

-- Get the Vulkan SDK path from the environment variable
local vulkan_SDK_EnvPath = os.getenv("VULKAN_SDK")

local dir = mainDir .. "/ThirdParty/Vulkan"

if(useEnv_Vulkan) then
	if not vulkan_SDK_EnvPath then
		error("VULKAN_SDK environment variable not set. Please install the Vulkan SDK. Falling back to ThirdParty version")
	else
		dir = vulkan_SDK_EnvPath
	end
end




vulkanIncludeDir = dir .. "/Include"
vulkanLibDir = dir .. "/Lib"
vulkanLibName = "vulkan-1"

vulkanDLLDir = dir .. "/DLL/vulkan-1.dll"

project "Vulkan"
	kind "SharedLib"
	language "C"
	location(projDir)
	
	files
	{
        dir .. "/**.h",
    }