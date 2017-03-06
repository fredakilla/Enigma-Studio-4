@echo off

SET BIN=..\..\..\..\tools\shadercRelease.exe

for /r %%i in (vs_*.hlsl) do (
echo Processing %%~nxi
shadercRelease.exe -f %%~nxi -o %%~ni.bin --varyingdef varying.def.sc --type v --platform windows --profile vs_5_0 --raw
)

for /r %%i in (ps_*.hlsl) do (
echo Processing %%~nxi
shadercRelease.exe -f %%~nxi -o %%~ni.bin --varyingdef varying.def.sc --type f --platform windows --profile ps_5_0 --raw
)

pause