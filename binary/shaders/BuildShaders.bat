@echo off

set OUTPWD=.
set PLATFORM=windows
set SHADERC="C:\github\bgfx\.build\win32_vs2015\bin\shadercRelease.exe"

IF NOT EXIST %OUTPWD% mkdir %OUTPWD%

set DIRSRC=Shaders\OpsWz4Shaders\TextureOperator

echo compiling TextureOperator
%SHADERC% -f  %DIRSRC%\OpsWz4_VS_RenderTexture.sc -o  %OUTPWD%\OpsWz4_VS_RenderTexture.bin --varyingdef %DIRSRC%\varying.def.sc --type v --platform %PLATFORM%
%SHADERC% -f  %DIRSRC%\OpsWz4_FS_RenderTexture.sc -o  %OUTPWD%\OpsWz4_FS_RenderTexture.bin --varyingdef %DIRSRC%\varying.def.sc --type f --platform %PLATFORM%


set DIRSRC=Shaders\OpsWz4Shaders\MeshOperator

echo compiling MeshOperator 1
%SHADERC% -f  %DIRSRC%\OpsWz4_VS_OperatorMesh.sc -o  %OUTPWD%\OpsWz4_VS_OperatorMesh.bin --varyingdef %DIRSRC%\varying.def.sc --type v --platform %PLATFORM%
echo compiling MeshOperator 2
%SHADERC% -f  %DIRSRC%\OpsWz4_FS_OperatorMesh.sc -o  %OUTPWD%\OpsWz4_FS_OperatorMesh.bin --varyingdef %DIRSRC%\varying.def.sc --type f --platform %PLATFORM%

pause

