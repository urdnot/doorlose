@echo off

cd /D "%~dp0"

if not exist "./addon" (
    mkdir addon 2>nul
)

if not exist "./addon/build" (
    mkdir build 2>nul
)
cd addon/build

echo on

cmake -G "Visual Studio 15 2017 Win64" ../doorlose
start doorlose.sln