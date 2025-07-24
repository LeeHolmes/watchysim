# watchysim

![image](https://user-images.githubusercontent.com/11475352/136731005-9394fdf6-fd35-4e0f-8e49-5a4b9101c643.png)

[Watchy](https://watchy.sqfmi.com/) is an exciting Open Source E-Paper Watch, based on Arduino and the ESP32 micro controller. WatchySim is a simulation framework for its watch faces.

## What problem does watchysim solve?

Watchy is based on Arduino, and so relies on using the Arduino IDE to compile and upload new Watchy sketches to define your watch face. This requires that you have the hardware available and connected, and is also slowed down somewhat by the speed of the Arduino upload process.

WatchySim gives you a Watchy-like SDK to develop your watch faces, but lets you test them in a standard Windows GDI-based application.

## Building WatchySim

WatchySim builds under Visual Studio [Community Edition](https://visualstudio.microsoft.com/vs/community/). This is a free version of Visual Studio that anybody can download and use.

### Linux Port

WatchySim now supports Linux through SDL2. The Linux port provides the same simulation capabilities with an interactive Dear ImGui menu for controlling various watch parameters.

<img width="1019" height="717" alt="image" src="https://github.com/user-attachments/assets/fd6024d3-ca62-47af-85cf-1ca9a0a27084" />

#### Linux Build Requirements

- CMake 3.10 or higher
- SDL2 development libraries
- C++11 compatible compiler (GCC or Clang)
- Git (for fetching Dear ImGui)

#### Linux Build Instructions

```bash
# Install dependencies (Ubuntu/Debian)
sudo apt-get install cmake libsdl2-dev build-essential

# Clone the repository
git clone https://github.com/LeeHolmes/watchysim.git
cd watchysim

# Build with CMake
mkdir build
cd build
cmake ..
make -j$(nproc)

# Run a specific watch face
./WatchySim_7_SEG    # or any other watch face binary
```

#### Linux Features

- All watch faces supported (7_SEG, DOS, Mario, Pokemon, etc.)
- Interactive ImGui menu (press 'M' to toggle)
- Button simulation (keys 1-4 or Q/W/E/R)
- Real-time parameter adjustment (battery, time, weather, etc.)
- Cross-platform compatibility

## Testing a Watch Face

WatchySim comes with most of the example Watchy watch faces by default. You can explore these examples to see how to add another. In general:

1) Add a new header file (for example, `Watchy_PowerShell.h`). This file defines your class that extends the Watchy class. Unlike the actual Watchy SDK, in this class - include `"Watchy.h"`, rather than `<Watchy.h>`. This now references our simulated version of the Watchy SDK. You will have to change this include to `<Watchy.h>` when you move this watch face to Arduino. In this file, most Watchy watch faces include a secondary header (for example `"powershell.h"`) that includes their bitmaps.
2) Add a new C++ file (for eample, `Watchy_PowerShell.cpp`) to implement the logic for your watch face. In this implementation, you will have access to many of the methods exposed by Watchy: `display.drawBitmap()`, `display.fillScreen()`, and more.
3) Update the beginning of `WatchySim.cpp` to include your new watch face's header file and create an instance of your custom watch face class. Follow the examples there, and comment out the ones that are currently defined.

## Known issues

- Since WatchySim is a simulation of the Watchy SDK, it implements the bare minimum of the Arduino SDK. Watch faces that rely on Arduino libraries will not work without you implementing (and hopefully contributing back a PR!) a simulation of those APIs.
- WatchySim is incomplete. It currently only implements enough of the Watchy SDK to get a set of popular / example watch faces building and simulating. If you want to simulate additional Watchy features that aren't currently implemented, you can add these to this project's version of `Watchy.h` and `Watchy.cpp`. Please contribute back a PR if you do, so the whole community can benefit.
- Some of what watchysim simulates might not be faithful to what happens in an actual physical Watchy. If you find inaccuracies, please contribute a PR!

## License
The contents of this repository is under an [MIT License](https://github.com/LeeHolmes/watchysim/blob/main/LICENSE).
