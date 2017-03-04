@echo off

set OUTPWD=.
set PLATFORM=windows
set SHADERC="shadercRelease.exe"

IF NOT EXIST %OUTPWD% mkdir %OUTPWD%

set DIRSRC=bgfx

echo compiling TextureOperator
%SHADERC% -f  %DIRSRC%\vs_cubes.sc -o  %OUTPWD%\vs_cubes.bin --varyingdef %DIRSRC%\varying.def.sc --type v --platform %PLATFORM%
%SHADERC% -f  %DIRSRC%\fs_cubes.sc -o  %OUTPWD%\fs_cubes.bin --varyingdef %DIRSRC%\varying.def.sc --type f --platform %PLATFORM%


REM set DIRSRC=Shaders\OpsWz4Shaders\MeshOperator

REM  echo compiling MeshOperator 1
REM %SHADERC% -f  %DIRSRC%\OpsWz4_VS_OperatorMesh.sc -o  %OUTPWD%\OpsWz4_VS_OperatorMesh.bin --varyingdef %DIRSRC%\varying.def.sc --type v --platform %PLATFORM%
REM echo compiling MeshOperator 2
REM  %SHADERC% -f  %DIRSRC%\OpsWz4_FS_OperatorMesh.sc -o  %OUTPWD%\OpsWz4_FS_OperatorMesh.bin --varyingdef %DIRSRC%\varying.def.sc --type f --platform %PLATFORM%

pause

