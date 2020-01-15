@echo off

if not exist "sources" mkdir sources
cd ./sources

if not exist "gtest" mkdir gtest
cd ./gtest

git clone https://github.com/google/googletest.git

for /F %%i in ('dir . /ad /b') do set gtest_dir=%%i

cd %gtest_dir%
mkdir build
cd build

echo on
cmake -DBUILD_SHARED_LIBS=ON -G "Visual Studio 15 2017 Win64" ..

@echo off
for /F %%i in ('dir /b *.sln') do set solution_file=%%i
echo on

call "C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Auxiliary/Build/vcvarsall.bat" x64
echo "Starting Build"
echo .  
devenv %solution_file% /build Debug
devenv %solution_file% /build Release
echo . 
echo "All builds completed." 
pause