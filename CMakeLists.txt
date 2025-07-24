cmake_minimum_required(VERSION 3.10)
project(WatchySim VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Options for different watch faces
option(BUILD_7SEG "Build 7-Segment watch face" ON)
option(BUILD_ANALOG_GABEL "Build Analog Gabel watch face" OFF)
option(BUILD_DOS "Build DOS watch face" OFF)
option(BUILD_MACPAINT "Build MacPaint watch face" OFF)
option(BUILD_MARIO "Build Mario watch face" OFF)
option(BUILD_POKEMON "Build Pokemon watch face" OFF)
option(BUILD_POWERSHELL "Build PowerShell watch face" OFF)
option(BUILD_TETRIS "Build Tetris watch face" OFF)
option(BUILD_BASIC "Build Basic watch face" OFF)
option(BUILD_NACHI "Build Nachi watch face" OFF)

# Find SDL2
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)

# Include directories
include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/WatchySim
    ${CMAKE_SOURCE_DIR}/WatchySim/arduino
    ${CMAKE_SOURCE_DIR}/external/imgui
    ${CMAKE_SOURCE_DIR}/external/imgui/backends
)

# Define macros
add_definitions(-DLINUX_BUILD)
add_definitions(-DSDL2_BACKEND)
add_definitions(-DWATCHY_SIM)

# Core source files
set(CORE_SOURCES
    WatchySim/WatchySim_SDL2.cpp
    WatchySim/Watchy_SDL2.cpp
    WatchySim/GraphicsAdapter_SDL2.cpp
    WatchySim/arduino/Common.cpp
    WatchySim/arduino/String.cpp
    WatchySim/arduino/dtostrf.cpp
    external/imgui/imgui.cpp
    external/imgui/imgui_demo.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/backends/imgui_impl_sdl2.cpp
    external/imgui/backends/imgui_impl_sdlrenderer2.cpp
)

# Core headers
set(CORE_HEADERS
    WatchySim/Watchy.h
    WatchySim/arduino.h
    WatchySim/config.h
    WatchySim/gfxfont.h
    WatchySim/arduino/Common.h
    WatchySim/arduino/String.h
    WatchySim/arduino/dtostrf.h
    WatchySim/arduino/pgmspace.h
)

# Function to add a watch face executable
function(add_watch_face NAME SOURCES)
    add_executable(WatchySim_${NAME} 
        ${CORE_SOURCES}
        ${SOURCES}
        ${ARGN}
    )
    
    target_compile_definitions(WatchySim_${NAME} PRIVATE USE_${NAME})
    target_link_libraries(WatchySim_${NAME} ${SDL2_LIBRARIES})
    target_compile_options(WatchySim_${NAME} PRIVATE ${SDL2_CFLAGS_OTHER})
    
    # Add include directories to handle relative includes properly
    target_include_directories(WatchySim_${NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/WatchySim
        ${CMAKE_SOURCE_DIR}/WatchySim/WatchFaces/${NAME}
    )
    
    # Add compiler warnings
    target_compile_options(WatchySim_${NAME} PRIVATE 
        -Wall -Wextra -Wpedantic
        $<$<CONFIG:Debug>:-g -O0>
        $<$<CONFIG:Release>:-O2>
    )
endfunction()

# Build selected watch faces
if(BUILD_7SEG)
    add_watch_face(7SEG
        WatchySim/WatchFaces/7_SEG/Watchy_7_SEG.cpp
    )
endif()

if(BUILD_ANALOG_GABEL)
    add_watch_face(AnalogGabel
        WatchySim/WatchFaces/AnalogGabel/Watchy_AnalogGabel.cpp
    )
endif()

if(BUILD_DOS)
    add_watch_face(DOS
        WatchySim/WatchFaces/DOS/Watchy_DOS.cpp
    )
endif()

if(BUILD_MACPAINT)
    add_watch_face(MacPaint
        WatchySim/WatchFaces/MacPaint/Watchy_MacPaint.cpp
    )
endif()

if(BUILD_MARIO)
    add_watch_face(Mario
        WatchySim/WatchFaces/Mario/Watchy_Mario.cpp
    )
endif()

if(BUILD_POKEMON)
    add_watch_face(Pokemon
        WatchySim/WatchFaces/Pokemon/Watchy_Pokemon.cpp
    )
endif()

if(BUILD_POWERSHELL)
    add_watch_face(PowerShell
        WatchySim/WatchFaces/PowerShell/Watchy_PowerShell.cpp
    )
endif()

if(BUILD_TETRIS)
    add_watch_face(Tetris
        WatchySim/WatchFaces/Tetris/Watchy_Tetris.cpp
    )
endif()

if(BUILD_BASIC)
    add_executable(WatchySim_Basic
        ${CORE_SOURCES}
        # Basic doesn't have its own cpp file, uses base Watchy
    )
    target_compile_definitions(WatchySim_Basic PRIVATE USE_BASIC)
    target_link_libraries(WatchySim_Basic ${SDL2_LIBRARIES})
    target_compile_options(WatchySim_Basic PRIVATE ${SDL2_CFLAGS_OTHER})
    target_compile_options(WatchySim_Basic PRIVATE 
        -Wall -Wextra -Wpedantic
        $<$<CONFIG:Debug>:-g -O0>
        $<$<CONFIG:Release>:-O2>
    )
endif()

if(BUILD_NACHI)
    add_watch_face(Nachi
        WatchySim/WatchFaces/Nachi/Watchy_Nachi.cpp
    )
endif()

# Default target that builds all enabled watch faces
add_custom_target(all_watchfaces)
if(BUILD_7SEG)
    add_dependencies(all_watchfaces WatchySim_7SEG)
endif()
if(BUILD_ANALOG_GABEL)
    add_dependencies(all_watchfaces WatchySim_AnalogGabel)
endif()
if(BUILD_DOS)
    add_dependencies(all_watchfaces WatchySim_DOS)
endif()
if(BUILD_MACPAINT)
    add_dependencies(all_watchfaces WatchySim_MacPaint)
endif()
if(BUILD_MARIO)
    add_dependencies(all_watchfaces WatchySim_Mario)
endif()
if(BUILD_POKEMON)
    add_dependencies(all_watchfaces WatchySim_Pokemon)
endif()
if(BUILD_POWERSHELL)
    add_dependencies(all_watchfaces WatchySim_PowerShell)
endif()
if(BUILD_TETRIS)
    add_dependencies(all_watchfaces WatchySim_Tetris)
endif()
if(BUILD_BASIC)
    add_dependencies(all_watchfaces WatchySim_Basic)
endif()
if(BUILD_NACHI)
    add_dependencies(all_watchfaces WatchySim_Nachi)
endif()

# Installation rules
if(BUILD_7SEG)
    install(TARGETS WatchySim_7SEG
        RUNTIME DESTINATION bin
    )
endif()
if(BUILD_ANALOG_GABEL)
    install(TARGETS WatchySim_AnalogGabel
        RUNTIME DESTINATION bin
    )
endif()
if(BUILD_DOS)
    install(TARGETS WatchySim_DOS
        RUNTIME DESTINATION bin
    )
endif()
if(BUILD_MACPAINT)
    install(TARGETS WatchySim_MacPaint
        RUNTIME DESTINATION bin
    )
endif()
if(BUILD_MARIO)
    install(TARGETS WatchySim_Mario
        RUNTIME DESTINATION bin
    )
endif()
if(BUILD_POKEMON)
    install(TARGETS WatchySim_Pokemon
        RUNTIME DESTINATION bin
    )
endif()
if(BUILD_POWERSHELL)
    install(TARGETS WatchySim_PowerShell
        RUNTIME DESTINATION bin
    )
endif()
if(BUILD_TETRIS)
    install(TARGETS WatchySim_Tetris
        RUNTIME DESTINATION bin
    )
endif()
if(BUILD_BASIC)
    install(TARGETS WatchySim_Basic
        RUNTIME DESTINATION bin
    )
endif()
if(BUILD_NACHI)
    install(TARGETS WatchySim_Nachi
        RUNTIME DESTINATION bin
    )
endif()

# Print configuration summary
message(STATUS "")
message(STATUS "WatchySim Linux Build Configuration:")
message(STATUS "  C++ Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "  SDL2 Version: ${SDL2_VERSION}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "")
message(STATUS "Watch Faces:")
message(STATUS "  7-Segment: ${BUILD_7SEG}")
message(STATUS "  Analog Gabel: ${BUILD_ANALOG_GABEL}")
message(STATUS "  DOS: ${BUILD_DOS}")
message(STATUS "  MacPaint: ${BUILD_MACPAINT}")
message(STATUS "  Mario: ${BUILD_MARIO}")
message(STATUS "  Pokemon: ${BUILD_POKEMON}")
message(STATUS "  PowerShell: ${BUILD_POWERSHELL}")
message(STATUS "  Tetris: ${BUILD_TETRIS}")
message(STATUS "  Basic: ${BUILD_BASIC}")
message(STATUS "  Nachi: ${BUILD_NACHI}")
message(STATUS "")