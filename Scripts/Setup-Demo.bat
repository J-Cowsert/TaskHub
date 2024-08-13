@echo off

pushd ..
Vendor\Binaries\Premake\Windows\premake5.exe --file=Build-Demo.lua vs2022
popd
pause