#!/bin/bash

# Build script for Luna LV2 Host
# Uses mingw-w64 for Windows cross-compilation

set -e

echo "Building Luna LV2 Host..."

# Check if mingw-w64 is available
if ! command -v x86_64-w64-mingw32-g++ &> /dev/null; then
    echo "Error: mingw-w64 cross-compiler not found"
    echo "Install it with: sudo apt install mingw-w64"
    exit 1
fi

# Check if win32 dependencies exist
if [ ! -d "win32" ]; then
    echo "Error: win32 directory not found"
    echo "Make sure Lilv and dependencies are available in the win32 directory"
    exit 1
fi

# Build using Makefile
echo "Compiling..."
make clean
make all

# Check build result
if [ -f "build/luna.exe" ]; then
    echo "Build completed successfully!"
    echo "Executable: build/luna.exe"
    
    # Install if requested
    if [ "$1" == "install" ]; then
        echo "Installing..."
        make install
        echo "Installed to: dist/bin/luna.exe"
    fi
else
    echo "Build failed!"
    exit 1
fi