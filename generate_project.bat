@echo off

set BUILD_DIR=build

cmake -S . -B %BUILD_DIR% -G "Visual Studio 17 2022"

pause
