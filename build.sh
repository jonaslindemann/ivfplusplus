#!/bin/bash
# Simple build script for Ivf++
# This script configures and builds both Debug and Release configurations
# vcpkg is auto-detected from VCPKG_ROOT or ~/vcpkg

echo ""
echo "******************************"
echo "*** Building Ivf++ library ***"
echo "******************************"
echo ""

echo "Configuring project..."
cmake -B build -G "Ninja Multi-Config"
if [ $? -ne 0 ]; then
    echo "Configuration failed!"
    exit 1
fi

echo ""
echo "Building Debug configuration..."
cmake --build build --config Debug
if [ $? -ne 0 ]; then
    echo "Debug build failed!"
    exit 1
fi

echo ""
echo "Building Release configuration..."
cmake --build build --config Release
if [ $? -ne 0 ]; then
    echo "Release build failed!"
    exit 1
fi

echo ""
echo "******************************"
echo "*** Build completed successfully ***"
echo "******************************"
echo ""
echo "Debug libraries are in: lib/ (with 'd' suffix)"
echo "Release libraries are in: lib/"
echo ""
