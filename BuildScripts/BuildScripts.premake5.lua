local dir = mainDir .. "/BuildScripts"

project "_BuildScripts"
    kind "Utility" -- or use "None"
    language "C++" -- or omit this if no compilation happens
    location(projDir)

    files
	{
		dir .. "/**.*"
    }