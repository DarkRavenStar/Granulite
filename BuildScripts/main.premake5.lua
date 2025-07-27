-- Include the helper functions
dofile("Helper.lua")

-- Get the current working directory
currentDir = os.getcwd()

-- Move up one directory by removing the last component of the path
mainDir = currentDir:match("(.*/)")
if mainDir then
    mainDir = mainDir:sub(1, -2)  -- Remove the trailing slash
else
    mainDir = currentDir
end

-- Print to verify
premake.info("Main Dir: " .. mainDir)
premake.info("Current Dir: " .. currentDir)

-- Define custom directories
-- Simplify this since testing on windows only
--outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
outputdir = ""

solutionDir = mainDir .. "/ProjectSetup"
targetDirMain = solutionDir .. "/Bin" .. outputdir .. "/%{prj.name}"
objDirMain = solutionDir .. "/Bin-Int" .. outputdir .. "/%{prj.name}"
projDir = solutionDir .. "/Projects/%{prj.name}"

workspace "Granulite"
   architecture "x64"
   configurations
   {
      "Debug",
      "Release"
   }
   
   location(solutionDir)  -- Set the solution directory

   startproject "Granulite"

   cppdialect "c++20"

   -- Do this globally unless need per project overide
   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "RELEASE" }
      optimize "On"
      runtime "Release"  -- Uses /MD
      linkoptions { "/NODEFAULTLIB:MSVCRT" }

   filter "system:windows"
      systemversion "latest"
      staticruntime "On"

   include "ZeroCheck.premake5.lua"
   include "BuildScripts.premake5.lua"
   include "GLFW.premake5.lua"
   include "Vulkan.premake5.lua"
   include "Volk.premake5.lua"
   include "Imgui.premake5.lua"
   include "VkBootstrap.premake5.lua"
   include "Granulite.premake5.lua"