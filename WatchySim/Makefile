# Makefile for WatchySim SDL2 Linux port

CXX = g++
CXXFLAGS = -std=c++11 -Wall -O2 -I. -I.. -I../external/imgui -I../external/imgui/backends -DWATCHY_SIM `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs`

# Object files
OBJS = WatchySim_SDL2.o \
       Watchy_SDL2.o \
       GraphicsAdapter_SDL2.o \
       arduino/Common.o \
       arduino/String.o \
       arduino/dtostrf.o \
       WatchFaces/7_SEG/Watchy_7_SEG.o \
       imgui.o \
       imgui_demo.o \
       imgui_draw.o \
       imgui_tables.o \
       imgui_widgets.o \
       imgui_impl_sdl2.o \
       imgui_impl_sdlrenderer2.o

# Target executable
TARGET = WatchySim

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compile source files
WatchySim_SDL2.o: WatchySim_SDL2.cpp Watchy_SDL2.h GraphicsAdapter.h
	$(CXX) $(CXXFLAGS) -c WatchySim_SDL2.cpp

Watchy_SDL2.o: Watchy_SDL2.cpp Watchy_SDL2.h GraphicsAdapter.h
	$(CXX) $(CXXFLAGS) -c Watchy_SDL2.cpp

GraphicsAdapter_SDL2.o: GraphicsAdapter_SDL2.cpp GraphicsAdapter.h
	$(CXX) $(CXXFLAGS) -c GraphicsAdapter_SDL2.cpp

arduino/Common.o: arduino/Common.cpp arduino/Common.h
	$(CXX) $(CXXFLAGS) -c arduino/Common.cpp -o arduino/Common.o

arduino/String.o: arduino/String.cpp arduino/String.h
	$(CXX) $(CXXFLAGS) -c arduino/String.cpp -o arduino/String.o

arduino/dtostrf.o: arduino/dtostrf.cpp arduino/dtostrf.h
	$(CXX) $(CXXFLAGS) -c arduino/dtostrf.cpp -o arduino/dtostrf.o

WatchFaces/7_SEG/Watchy_7_SEG.o: WatchFaces/7_SEG/Watchy_7_SEG.cpp WatchFaces/7_SEG/Watchy_7_SEG.h
	$(CXX) $(CXXFLAGS) -c WatchFaces/7_SEG/Watchy_7_SEG.cpp -o WatchFaces/7_SEG/Watchy_7_SEG.o

# ImGui object files
imgui.o: ../external/imgui/imgui.cpp
	$(CXX) $(CXXFLAGS) -c ../external/imgui/imgui.cpp

imgui_demo.o: ../external/imgui/imgui_demo.cpp
	$(CXX) $(CXXFLAGS) -c ../external/imgui/imgui_demo.cpp

imgui_draw.o: ../external/imgui/imgui_draw.cpp
	$(CXX) $(CXXFLAGS) -c ../external/imgui/imgui_draw.cpp

imgui_tables.o: ../external/imgui/imgui_tables.cpp
	$(CXX) $(CXXFLAGS) -c ../external/imgui/imgui_tables.cpp

imgui_widgets.o: ../external/imgui/imgui_widgets.cpp
	$(CXX) $(CXXFLAGS) -c ../external/imgui/imgui_widgets.cpp

imgui_impl_sdl2.o: ../external/imgui/backends/imgui_impl_sdl2.cpp
	$(CXX) $(CXXFLAGS) -c ../external/imgui/backends/imgui_impl_sdl2.cpp

imgui_impl_sdlrenderer2.o: ../external/imgui/backends/imgui_impl_sdlrenderer2.cpp
	$(CXX) $(CXXFLAGS) -c ../external/imgui/backends/imgui_impl_sdlrenderer2.cpp

# Clean build files
clean:
	rm -f $(OBJS) $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run