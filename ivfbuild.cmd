@echo off 

set DEBUG_DIR=build-debug
set RELEASE_DIR=build-release

echo.
echo ******************************
echo *** Building Ivf++ library ***
echo ******************************
echo.
echo NOTE: This script uses automatic vcpkg detection.
echo Vcpkg will be detected from VCPKG_ROOT or common locations (c:/vcpkg, e:/vcpkg).
echo To use a custom location, set VCPKG_ROOT environment variable before running this script.
echo.

echo.
echo *** Configuring debug build
echo.
cmake -B %DEBUG_DIR% -S . -G "Visual Studio 17 2022" -DIVF_DEBUG=ON
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Debug configuration failed
    exit /b %ERRORLEVEL%
)

echo.
echo *** Building debug build
echo.
cmake --build %DEBUG_DIR% --config Debug -- /m
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Debug build failed
    exit /b %ERRORLEVEL%
)

echo.
echo *** Configuring release build
echo.
cmake -B %RELEASE_DIR% -S . -G "Visual Studio 17 2022"
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Release configuration failed
    exit /b %ERRORLEVEL%
)

echo.
echo *** Building release build
echo.
cmake --build %RELEASE_DIR% --config Release -- /m
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Release build failed
    exit /b %ERRORLEVEL%
)

echo.
echo *** Build completed successfully! ***
echo Debug libraries: lib\Debug
echo Release libraries: lib\Release
echo.




