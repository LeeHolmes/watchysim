# watchysim

![image](https://user-images.githubusercontent.com/11475352/136417949-7311f508-e30f-44e6-bc8c-1b6356391316.png)


[Watchy](https://watchy.sqfmi.com/) is an exciting Open Source E-Paper Watch, based on Arduino and the ESP32 micro controller. WatchySim is a simulation framework for its watch faces.

## What problem does watchysim solve?

Watchy is based on Arduino, and so relies on using the Arduino IDE to compile and upload new Watchy sketches to define your watch face. This requires that you have the hardware available and connected, and is also slowed down somewhat by the speed of the Arduino upload process.

WatchySim gives you a Watchy-like SDK to develop your watch faces, but lets you test them in a standard Windows GDI-based application.

## Building WatchySim

WatchySim builds under Visual Studio 2017 Community Edition.

## Testing a Watch Face

WatchySim comes with most of the example Watchy watch faces by default. You can explore these examples to see how to add another. In general:

1) Add a new header file (for example, `Watchy_PowerShell.h`). This file defines your class that extends the Watchy class. Unlike the actual Watchy SDK, in this class - include `"Watchy.h"`, rather than `<Watchy.h>`. This now references our simulated version of the Watchy SDK. You will have to change this include to `<Watchy.h>` when you move this watch face to Arduino. In this file, most Watchy watch faces include a secondary header (for example `"powershell.h"`) that includes their bitmaps.
2) Add a new C++ file (for eample, `Watchy_PowerShell.cpp`) to implement the logic for your watch face. In this implementation, you will have access to many of the methods exposed by Watchy: `display.drawBitmap()`, `display.fillScreen()`, and more.
3) Update the beginning of `WatchySim.cpp` to include your new watch face's header file and create an instance of your custom watch face class. Follow the examples there, and comment out the ones that are currently defined.

## Known issues

- Since WatchySim is a simulation of the Watchy SDK, it implements the bare minimum of the Arduino SDK. Watch faces that rely on Arduino libraries will not work without you implementing (and hopefully contributing back a PR!) a simulation of those APIs.
- WatchySim is incomplete. It currently only implements enough of the Watchy SDK to get a set of popular / example watch faces building and simulating. If you want to simulate additional Watchy features that aren't currently implemented, you can add these to this project's version of `Watchy.h` and `Watchy.cpp`. Please contribute back a PR if you do, so the whole community can benefit.
- Some of what watchysim simulates might not be faithful to what happens in an actual physical Watchy. If you find inaccuracies, please contribute a PR!
