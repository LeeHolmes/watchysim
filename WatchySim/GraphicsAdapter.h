/*
 * GraphicsAdapter.h - Cross-platform graphics adapter layer
 * 
 * This provides a common interface that can be implemented by both
 * GDI+ (Windows) and SDL2 (Linux) backends.
 */

#ifndef GRAPHICS_ADAPTER_H
#define GRAPHICS_ADAPTER_H

#include <stdint.h>

// Forward declarations for platform-specific types
#ifdef _WIN32
    namespace Gdiplus {
        class Graphics;
    }
    typedef void* HDC;
#else
    // SDL2 types
    struct SDL_Renderer;
    struct SDL_Texture;
#endif

// Abstract graphics context interface
class GraphicsContext {
public:
    virtual ~GraphicsContext() {}
    
    // Drawing primitives
    virtual void setPixel(int x, int y, uint32_t color) = 0;
    virtual void drawLine(int x0, int y0, int x1, int y1, uint32_t color) = 0;
    virtual void fillRect(int x, int y, int w, int h, uint32_t color) = 0;
    
    // Text rendering
    virtual void drawText(const char* text, int x, int y, uint32_t color) = 0;
    virtual void measureText(const char* text, int* width, int* height) = 0;
    
    // Update display
    virtual void present() = 0;
};

#ifdef _WIN32
// Windows implementation using GDI+
class GDIPlusGraphicsContext : public GraphicsContext {
private:
    Gdiplus::Graphics* graphics;
    HDC* hdc;
    
public:
    GDIPlusGraphicsContext(Gdiplus::Graphics* g, HDC* h) : graphics(g), hdc(h) {}
    
    void setPixel(int x, int y, uint32_t color) override;
    void drawLine(int x0, int y0, int x1, int y1, uint32_t color) override;
    void fillRect(int x, int y, int w, int h, uint32_t color) override;
    void drawText(const char* text, int x, int y, uint32_t color) override;
    void measureText(const char* text, int* width, int* height) override;
    void present() override;
};

#else
// Linux implementation using SDL2
class SDL2GraphicsContext : public GraphicsContext {
private:
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    uint32_t* pixels;
    int width;
    int height;
    
public:
    SDL2GraphicsContext(SDL_Renderer* r, SDL_Texture* t, uint32_t* p, int w, int h) 
        : renderer(r), texture(t), pixels(p), width(w), height(h) {}
    
    void setPixel(int x, int y, uint32_t color) override;
    void drawLine(int x0, int y0, int x1, int y1, uint32_t color) override;
    void fillRect(int x, int y, int w, int h, uint32_t color) override;
    void drawText(const char* text, int x, int y, uint32_t color) override;
    void measureText(const char* text, int* width, int* height) override;
    void present() override;
};
#endif

// Color conversion utilities
inline uint32_t colorFromRGB(uint8_t r, uint8_t g, uint8_t b) {
    return (0xFF << 24) | (r << 16) | (g << 8) | b;
}

inline uint32_t colorFromGxEPD(uint16_t color) {
    // GxEPD_BLACK = 0x0000, GxEPD_WHITE = 0xFFFF
    return (color == 0x0000) ? colorFromRGB(0, 0, 0) : colorFromRGB(255, 255, 255);
}

#endif // GRAPHICS_ADAPTER_H