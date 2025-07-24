#!/bin/bash

# Build script for all watch faces
# This script will build each watch face and report any build errors

echo "Building all watch faces..."
echo "=========================="

# Array of watch faces and their corresponding defines
declare -A watchfaces=(
    ["7_SEG"]="USE_7SEG"
    ["AnalogGabel"]="USE_AnalogGabel"
    ["Basic"]="USE_BASIC"
    ["DOS"]="USE_DOS"
    ["MacPaint"]="USE_MacPaint"
    ["Mario"]="USE_Mario"
    ["Pokemon"]="USE_Pokemon"
    ["PowerShell"]="USE_PowerShell"
    ["Tetris"]="USE_Tetris"
)

# Track failures
failed_builds=()

# Build each watch face
for face in "${!watchfaces[@]}"; do
    echo -e "\n=== Building $face ==="
    
    # For Basic, we need to remove the watch face object from OBJS
    if [ "$face" = "Basic" ]; then
        # Create a modified Makefile without the watch face object
        cp Makefile Makefile.tmp
        # Remove the watch face object line from OBJS
        sed -i '/WatchFaces.*Watchy.*\.o/d' Makefile.tmp
        # Remove the watch face build rule
        sed -i '/^WatchFaces.*Watchy.*\.o:/,/^\t/d' Makefile.tmp
    else
        # Other watch faces need their specific object file
        cp Makefile Makefile.tmp
        # Update the watch face object in OBJS
        sed -i "s|WatchFaces/.*/Watchy_.*\.o|WatchFaces/$face/Watchy_$face.o|g" Makefile.tmp
        # Update the build rule
        sed -i "s|WatchFaces/.*/Watchy_.*\.cpp|WatchFaces/$face/Watchy_$face.cpp|g" Makefile.tmp
        sed -i "s|WatchFaces/.*/Watchy_.*\.h|WatchFaces/$face/Watchy_$face.h|g" Makefile.tmp
    fi
    
    # Clean previous build
    make -f Makefile.tmp clean > /dev/null 2>&1
    
    # Build with the appropriate define
    if make -f Makefile.tmp CXXFLAGS="-std=c++11 -Wall -O2 -I. -I.. -DWATCHY_SIM -D${watchfaces[$face]} \`sdl2-config --cflags\`" > build_$face.log 2>&1; then
        echo "✓ $face built successfully"
    else
        echo "✗ $face build failed"
        failed_builds+=("$face")
        echo "  Error log saved to build_$face.log"
    fi
    
    rm -f Makefile.tmp
done

# Summary
echo -e "\n=========================="
echo "Build Summary:"
echo "=========================="
if [ ${#failed_builds[@]} -eq 0 ]; then
    echo "All watch faces built successfully!"
else
    echo "Failed builds: ${failed_builds[@]}"
    echo "Check the corresponding build_*.log files for details"
fi