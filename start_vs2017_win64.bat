@echo off

cd /D "%~dp0"

if not exist "./build" (
    mkdir build 2>nul
)
cd build || (
    echo ERROR: Failed to create build directory
    goto BAD
)

echo on

cmake -G "Visual Studio 15 2017 Win64" ../doorlose
"doorlose.sln"

echo Start project SUCCEEDED.
exit /B 0

:BAD
echo start project FAILED.
exit /B 1