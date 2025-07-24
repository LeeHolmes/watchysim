# WatchySim Linux Port

This branch adds Linux support to WatchySim using SDL2, while maintaining full Windows compatibility.

## Requirements

- SDL2 development libraries
- C++11 compatible compiler (g++ or clang++)
- Make or CMake

### Installing Dependencies

**Ubuntu/Debian:**
```bash
sudo apt-get install libsdl2-dev build-essential
```

**Fedora:**
```bash
sudo dnf install SDL2-devel gcc-c++ make
```

**Arch Linux:**
```bash
sudo pacman -S sdl2 base-devel
```

## Building

### Quick Build
```bash
cd WatchySim
make
```

### Using the build script
```bash
chmod +x build_linux.sh
./build_linux.sh
```

### Using CMake
```bash
mkdir build
cd build
cmake ..
make
```

## Running

```bash
cd WatchySim
./WatchySim
```

## Controls

- **ESC** - Exit
- **R** - Refresh display
- **T** - Test pattern
- **1/Q** - Button 1 (Menu/Back)
- **2/W** - Button 2 (Up)
- **3/E** - Button 3 (Down)
- **4** - Button 4 (Select)

## Architecture

The Linux port uses a platform abstraction layer that allows the same codebase to compile on both Windows and Linux:

- **Windows**: Uses GDI+ for rendering (original implementation)
- **Linux**: Uses SDL2 for rendering (new implementation)
- **Shared**: Watch faces and Arduino compatibility layer work on both platforms

Key files:
- `GraphicsAdapter.h` - Platform abstraction interface
- `WatchySim_SDL2.cpp` - Linux main program
- `Watchy_SDL2.cpp/h` - Linux Watchy implementation
- `GraphicsAdapter_SDL2.cpp` - SDL2 graphics implementation

## Limitations

- Menu system not yet implemented (coming soon with Dear ImGui)
- Background image not displayed (requires SDL2_image)
- Basic font rendering (placeholder rectangles)

## Compatibility

All existing watch faces work without modification. The platform abstraction ensures pixel-perfect rendering across both Windows and Linux.