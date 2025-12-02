@echo off
REM Simple build script for Ivf++
REM This script configures and builds both Debug and Release configurations
REM vcpkg is auto-detected from VCPKG_ROOT or c:/vcpkg

echo.
echo ******************************
echo *** Building Ivf++ library ***
echo ******************************
echo.

echo Configuring project...
cmake -B build -G "Ninja Multi-Config"
if errorlevel 1 (
    echo Configuration failed!
    exit /b 1
)

echo.
echo Building Debug configuration...
cmake --build build --config Debug
if errorlevel 1 (
    echo Debug build failed!
    exit /b 1
)

echo.
echo Building Release configuration...
cmake --build build --config Release
if errorlevel 1 (
    echo Release build failed!
    exit /b 1
)

echo.
echo ******************************
echo *** Build completed successfully ***
echo ******************************
echo.
echo Debug libraries are in: lib\ (with 'd' suffix)
echo Release libraries are in: lib\
echo.
