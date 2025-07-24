#!/bin/bash

# WatchySim Linux Build Script
# This script configures and builds WatchySim for Linux using CMake and SDL2

set -e  # Exit on error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to print colored output
print_info() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

# Function to check if a command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Function to display usage
usage() {
    cat << EOF
Usage: $0 [OPTIONS]

Build WatchySim for Linux with SDL2 backend.

OPTIONS:
    -h, --help              Show this help message
    -c, --clean             Clean build directory before building
    -d, --debug             Build in debug mode (default: release)
    -a, --all               Build all watch faces
    -f, --face FACE         Build specific watch face (7seg, analog, dos, macpaint, mario, pokemon, powershell, tetris, basic)
    -j, --jobs N            Number of parallel build jobs (default: auto)
    -i, --install           Install after building (requires sudo)

EXAMPLES:
    $0                      # Build default watch face (7-Segment)
    $0 -a                   # Build all watch faces
    $0 -f mario -d          # Build Mario watch face in debug mode
    $0 -c -a -j 4           # Clean build, all faces, 4 parallel jobs

EOF
}

# Default values
CLEAN_BUILD=false
BUILD_TYPE="Release"
BUILD_ALL=false
SPECIFIC_FACE=""
NUM_JOBS=""
DO_INSTALL=false

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -h|--help)
            usage
            exit 0
            ;;
        -c|--clean)
            CLEAN_BUILD=true
            shift
            ;;
        -d|--debug)
            BUILD_TYPE="Debug"
            shift
            ;;
        -a|--all)
            BUILD_ALL=true
            shift
            ;;
        -f|--face)
            SPECIFIC_FACE="$2"
            shift 2
            ;;
        -j|--jobs)
            NUM_JOBS="$2"
            shift 2
            ;;
        -i|--install)
            DO_INSTALL=true
            shift
            ;;
        *)
            print_error "Unknown option: $1"
            usage
            exit 1
            ;;
    esac
done

# Check dependencies
print_info "Checking dependencies..."

if ! command_exists cmake; then
    print_error "CMake is not installed. Please install it:"
    echo "  Ubuntu/Debian: sudo apt-get install cmake"
    echo "  Fedora: sudo dnf install cmake"
    echo "  Arch: sudo pacman -S cmake"
    exit 1
fi

if ! command_exists pkg-config; then
    print_error "pkg-config is not installed. Please install it:"
    echo "  Ubuntu/Debian: sudo apt-get install pkg-config"
    echo "  Fedora: sudo dnf install pkgconfig"
    echo "  Arch: sudo pacman -S pkg-config"
    exit 1
fi

if ! pkg-config --exists sdl2; then
    print_error "SDL2 is not installed. Please install it:"
    echo "  Ubuntu/Debian: sudo apt-get install libsdl2-dev"
    echo "  Fedora: sudo dnf install SDL2-devel"
    echo "  Arch: sudo pacman -S sdl2"
    exit 1
fi

# Determine number of jobs
if [ -z "$NUM_JOBS" ]; then
    if command_exists nproc; then
        NUM_JOBS=$(nproc)
    else
        NUM_JOBS=1
    fi
fi

print_info "Build configuration:"
echo "  Build type: $BUILD_TYPE"
echo "  Parallel jobs: $NUM_JOBS"
echo "  SDL2 version: $(pkg-config --modversion sdl2)"

# Create build directory
BUILD_DIR="build"
if [ "$CLEAN_BUILD" = true ] && [ -d "$BUILD_DIR" ]; then
    print_info "Cleaning build directory..."
    rm -rf "$BUILD_DIR"
fi

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Configure CMake options
CMAKE_OPTS="-DCMAKE_BUILD_TYPE=$BUILD_TYPE"

if [ "$BUILD_ALL" = true ]; then
    CMAKE_OPTS="$CMAKE_OPTS -DBUILD_7SEG=ON -DBUILD_ANALOG_GABEL=ON -DBUILD_DOS=ON"
    CMAKE_OPTS="$CMAKE_OPTS -DBUILD_MACPAINT=ON -DBUILD_MARIO=ON -DBUILD_POKEMON=ON"
    CMAKE_OPTS="$CMAKE_OPTS -DBUILD_POWERSHELL=ON -DBUILD_TETRIS=ON -DBUILD_BASIC=ON"
elif [ -n "$SPECIFIC_FACE" ]; then
    # Disable all faces first
    CMAKE_OPTS="$CMAKE_OPTS -DBUILD_7SEG=OFF -DBUILD_ANALOG_GABEL=OFF -DBUILD_DOS=OFF"
    CMAKE_OPTS="$CMAKE_OPTS -DBUILD_MACPAINT=OFF -DBUILD_MARIO=OFF -DBUILD_POKEMON=OFF"
    CMAKE_OPTS="$CMAKE_OPTS -DBUILD_POWERSHELL=OFF -DBUILD_TETRIS=OFF -DBUILD_BASIC=OFF"
    
    # Enable specific face
    case "$SPECIFIC_FACE" in
        7seg)
            CMAKE_OPTS="$CMAKE_OPTS -DBUILD_7SEG=ON"
            ;;
        analog)
            CMAKE_OPTS="$CMAKE_OPTS -DBUILD_ANALOG_GABEL=ON"
            ;;
        dos)
            CMAKE_OPTS="$CMAKE_OPTS -DBUILD_DOS=ON"
            ;;
        macpaint)
            CMAKE_OPTS="$CMAKE_OPTS -DBUILD_MACPAINT=ON"
            ;;
        mario)
            CMAKE_OPTS="$CMAKE_OPTS -DBUILD_MARIO=ON"
            ;;
        pokemon)
            CMAKE_OPTS="$CMAKE_OPTS -DBUILD_POKEMON=ON"
            ;;
        powershell)
            CMAKE_OPTS="$CMAKE_OPTS -DBUILD_POWERSHELL=ON"
            ;;
        tetris)
            CMAKE_OPTS="$CMAKE_OPTS -DBUILD_TETRIS=ON"
            ;;
        basic)
            CMAKE_OPTS="$CMAKE_OPTS -DBUILD_BASIC=ON"
            ;;
        *)
            print_error "Unknown watch face: $SPECIFIC_FACE"
            echo "Valid options: 7seg, analog, dos, macpaint, mario, pokemon, powershell, tetris, basic"
            exit 1
            ;;
    esac
fi

# Configure
print_info "Configuring build..."
cmake .. $CMAKE_OPTS

# Build
print_info "Building WatchySim..."
cmake --build . -j "$NUM_JOBS"

# Install if requested
if [ "$DO_INSTALL" = true ]; then
    print_info "Installing WatchySim (may require sudo)..."
    sudo cmake --install .
fi

# Print success message and instructions
print_info "Build completed successfully!"
echo ""
echo "Executables built:"
find . -name "WatchySim_*" -type f -executable | while read -r exe; do
    echo "  - $exe"
done

echo ""
echo "To run a watch face:"
echo "  cd build"
echo "  ./WatchySim_7SEG"
echo ""
echo "Note: The Linux port requires SDL2 implementations of WatchySim_SDL2.cpp and Watchy_SDL2.cpp"
echo "      These files need to be created based on the Windows versions with SDL2 drawing calls."
echo ""

# Check if required SDL2 files exist
cd ..
if [ ! -f "WatchySim/WatchySim_SDL2.cpp" ] || [ ! -f "WatchySim/Watchy_SDL2.cpp" ]; then
    print_warning "SDL2 implementation files not found!"
    echo "Please create:"
    echo "  - WatchySim/WatchySim_SDL2.cpp (main application with SDL2)"
    echo "  - WatchySim/Watchy_SDL2.cpp (display driver with SDL2)"
    echo ""
    echo "Refer to linux_port_dev/sdl2_poc.cpp and GDI_TO_SDL2_MAPPING.md for implementation guidance."
fi