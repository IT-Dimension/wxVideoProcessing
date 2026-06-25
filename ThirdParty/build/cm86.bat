rem @echo off
set OPENAL_LIB_DIR=%CD%\..\OpenAL
echo %OPENAL_LIB_DIR%
rmdir /S /Q Win
rem copy "..\freealut\admin\CMakeModules\*.cmake" .
mkdir Win
cd Win
cmake ../ -G "Visual Studio 15 2017" -DCPU_ARCH=x86
cd ..
msbuild /m /t:Rebuild /p:Configuration=Debug /p:Platform="Win32" Win\iFloorThirdParty.sln
msbuild /m /t:Rebuild /p:Configuration=Release /p:Platform="Win32" Win\iFloorThirdParty.sln
