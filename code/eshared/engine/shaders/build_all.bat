@echo off

SET BIN=..\..\..\..\tools\shader_minifier.exe

for /r %%i in (*.hlsl) do (
echo Processing %%~nxi
%BIN% --hlsl --preserve-all-globals -o %%~ni.hpp %%~nxi
)

for /r %%i in (*.hlsli) do (
echo Processing %%~nxi
%BIN% --hlsl --preserve-all-globals -o %%~ni.hpp %%~nxi
)
