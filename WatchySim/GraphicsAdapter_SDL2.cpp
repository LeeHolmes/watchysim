/*
 * GraphicsAdapter_SDL2.cpp - SDL2 implementation of the graphics adapter
 */

#ifndef _WIN32

#include "GraphicsAdapter.h"
#include <SDL2/SDL.h>
#include <cstring>
#include <algorithm>
#include <iostream>

void SDL2GraphicsContext::setPixel(int x, int y, uint32_t color) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        pixels[y * width + x] = color;
    }
}

void SDL2GraphicsContext::drawLine(int x0, int y0, int x1, int y1, uint32_t color) {
    // Bresenham's line algorithm
    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;
    
    while (true) {
        setPixel(x0, y0, color);
        
        if (x0 == x1 && y0 == y1) break;
        
        int e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void SDL2GraphicsContext::fillRect(int x, int y, int w, int h, uint32_t color) {
    for (int j = y; j < y + h && j < height; j++) {
        for (int i = x; i < x + w && i < width; i++) {
            if (i >= 0 && j >= 0) {
                setPixel(i, j, color);
            }
        }
    }
}

void SDL2GraphicsContext::drawText(const char* text, int x, int y, uint32_t color) {
    // TODO: Implement proper text rendering
    // For now, just draw a placeholder rectangle where text would be
    int textWidth = strlen(text) * 6;  // Approximate width
    int textHeight = 8;  // Approximate height
    
    // Draw a filled rectangle to represent text
    fillRect(x, y - textHeight, textWidth, textHeight, color);
}

void SDL2GraphicsContext::measureText(const char* text, int* width, int* height) {
    // TODO: Implement proper text measurement
    // For now, use approximate values
    if (width) *width = strlen(text) * 6;
    if (height) *height = 8;
}

void SDL2GraphicsContext::present() {
    // Update the texture with the pixel data
    int result = SDL_UpdateTexture(texture, NULL, pixels, width * sizeof(uint32_t));
    if (result != 0) {
        std::cerr << "[SDL2 ERROR] SDL_UpdateTexture failed: " << SDL_GetError() << std::endl;
    } else {
    }
    
    // Note: The actual rendering to screen should be done by the main loop
    // This just updates the texture with the current pixel data
}

#endif // !_WIN32