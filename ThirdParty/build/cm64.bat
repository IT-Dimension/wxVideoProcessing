rem @echo off
set OPENAL_LIB_DIR=%CD%\..\OpenAL
echo %OPENAL_LIB_DIR%
rmdir /S /Q Win
mkdir Win
cd Win
cmake ../ -G "Visual Studio 15 2017 Win64" -DCPU_ARCH=x64
cd ..
msbuild /t:Rebuild /p:Configuration=Debug /p:Platform="x64" Win\iFloorThirdParty.sln
msbuild /t:Rebuild /p:Configuration=Release /p:Platform="x64" Win\iFloorThirdParty.sln
