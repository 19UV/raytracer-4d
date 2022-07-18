@echo off

echo Building in Production Mode

cmake -B build

cmake --build build --target ALL_BUILD --config Release
