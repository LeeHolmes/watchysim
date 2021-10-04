# watchysim

![image](https://user-images.githubusercontent.com/11475352/135791129-989d4508-b512-4a5c-8e2a-ce300b0291c6.png)

WatchySim is a simulation framework for watchy watch faces.

Rather than using the Arduino IDE to upload new Watchy sketches to define your watch face, WatchySim gives you a Watchy-like SDK to develop your watch faces in a standard Windows GDI-based application.

## Building WatchySim

WatchySim builds under Visual Studio 2017 Community Edition.

## Testing a Watch Face

WatchySim comes with most of the example Watchy watch faces by default. You can explore these examples to see how to add another. In general:

1) Add a new header file (for example, Watchy_PowerShell.h). This file defines your class that extends the Watchy class. Unlike the actual Watchy SDK, in this class - include "Watchy.h", rather than <Watchy.h>. This now references our simulated version of the Watchy SDK. You will have to change this include to <Watchy.h> when you move this watch face to Arduino. In this file, most Watchy watch faces include a secondary header (for example "powershell.h") that includes their bitmaps.
2) Add a new C++ file (for eample, Watchy_PowerShell.cpp) to implement the logic for your watch face. In this implementation, you will have access to many of the methods exposed by Watchy: `display.drawBitmap()`, `display.fillScreen()`, and more.
3) Update the beginning of WatchySim.cpp to include your new watch face's header file and create an instance of your custom watch face class. Follow the examples there, and comment out the ones that are currently define.

## Known issues

Since WatchySim is merely a simulation of the Watchy SDK, it implements the bare minimum of the Arduino SDK. It also only implements enough of the Watchy SDK to get a set of popular / example watch faces building and simulating. If you want to simulate additional Watchy features that aren't currently implemented, you can add these to this project's version of Watchy.h and Watchy.cpp.
