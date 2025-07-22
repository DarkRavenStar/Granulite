
local useGlfw_Lib = true
local dir = mainDir .. "/ThirdParty/" .. (useGlfw_Lib and "glfw_lib" or "glfw")
local sourceDir = mainDir .. "/ThirdParty/glfw/src"
glfwIncludeDir = dir .. "/include"
glfwLibDir = dir .. "/lib-vc2022"
glfwLibName = "glfw3_mt" --We are going with mt version for simplicity - glfw3

project "GLFW"
	kind "StaticLib"
	language "C++"
	location(projDir)
	
	files
	{
        glfwIncludeDir .. "/**.h",
		sourceDir .. "/**.*"
    }
	
	filter { "files:" .. path.getrelative(os.getcwd(), sourceDir) .. "/**.*" }
		flags { "ExcludeFromBuild" }

	filter {} -- Clear filter
