@echo off

"%~dp0\ThirdParty\Premake\premake5.exe" vs2022 --file="%~dp0\BuildScripts/main.premake5.lua" --verbose

pause