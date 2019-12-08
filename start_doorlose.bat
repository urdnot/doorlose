@echo off

if not exist "./build" mkdir build
cd build

echo on

cmake -G "Visual Studio 15 2017" ../doorlose