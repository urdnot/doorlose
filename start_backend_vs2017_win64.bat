@echo off

cd /D "%~dp0"

if not exist "./build" (
    mkdir build 2>nul
)

cd build

if not exist "./backend" (
    mkdir backend 2>nul
)

cd backend

echo on

cmake -G "Visual Studio 15 2017 Win64" ../../backend
start backend.sln