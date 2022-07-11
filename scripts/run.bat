@echo off

if not exist build/ cmake -B build

cmake --build build && build\Debug\raytracer4d.exe
echo Return Code: %errorlevel%
