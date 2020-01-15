@echo off

cd /D "%~dp0"

if not exist "./build" (
    mkdir build 2>nul
)

cd build

echo on

cmake -G "Visual Studio 15 2017 Win64" ../backend
start backend.sln