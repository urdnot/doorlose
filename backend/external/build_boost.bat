@echo off

cd ./sources/boost

for /F %%i in ('dir . /ad /b') do set boost_dir=%%i

cd %boost_dir%

echo on

call bootstrap.bat
call b2.exe -j 8 toolset=msvc address-model=64 architecture=x86 variant=debug,release link=shared --build-dir=build --build-type=complete --abbreviate-paths
pause