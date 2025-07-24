/*
 * WatchySim_SDL2.cpp - Linux port of WatchySim using SDL2
 * 
 * This is a port of the Windows WatchySim to Linux using SDL2.
 * Menu functionality is temporarily commented out (to be added with Dear ImGui later).
 * 
 * Compile with: g++ -o WatchySim_SDL2 WatchySim_SDL2.cpp `sdl2-config --cflags --libs`
 */

#include <SDL2/SDL.h>
#include <cstdint>
#include <cstring>
#include <ctime>
#include <iostream>

// Dear ImGui includes
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

// Include Watchy headers
#ifndef _WIN32
#include "Watchy_SDL2.h"
#else
#include "Watchy.h"
#endif
#include "GraphicsAdapter.h"

// Watch faces - conditional includes based on build configuration
#ifdef USE_7SEG
#include "WatchFaces/7_SEG/Watchy_7_SEG.h"
#elif defined(USE_AnalogGabel)
#include "WatchFaces/AnalogGabel/Watchy_AnalogGabel.h"
#elif defined(USE_DOS)
#include "WatchFaces/DOS/Watchy_DOS.h"
#elif defined(USE_MacPaint)
#include "WatchFaces/MacPaint/Watchy_MacPaint.h"
#elif defined(USE_Mario)
#include "WatchFaces/Mario/Watchy_Mario.h"
#elif defined(USE_Pokemon)
#include "WatchFaces/Pokemon/Watchy_Pokemon.h"
#elif defined(USE_PowerShell)
#include "WatchFaces/PowerShell/Watchy_PowerShell.h"
#elif defined(USE_Tetris)
#include "WatchFaces/Tetris/Watchy_Tetris.h"
#elif defined(USE_BASIC)
// Basic uses the base Watchy class
#elif defined(USE_Nachi)
#include "WatchFaces/Nachi/Watchy_Nachi.h"
#else
// Default to 7-segment if nothing is defined
#include "WatchFaces/7_SEG/Watchy_7_SEG.h"
#endif

// Initialize the watch face based on build configuration
#ifdef USE_7SEG
Watchy7SEG watchy = Watchy7SEG();
#elif defined(USE_AnalogGabel)
WatchyAnalogGabel watchy = WatchyAnalogGabel();
#elif defined(USE_DOS)
WatchyDOS watchy = WatchyDOS();
#elif defined(USE_MacPaint)
WatchyMacPaint watchy = WatchyMacPaint();
#elif defined(USE_Mario)
WatchyMario watchy = WatchyMario();
#elif defined(USE_Pokemon)
WatchyPokemon watchy = WatchyPokemon();
#elif defined(USE_PowerShell)
WatchyPowerShell watchy = WatchyPowerShell();
#elif defined(USE_Tetris)
WatchyTetris watchy = WatchyTetris();
#elif defined(USE_BASIC)
Watchy watchy = Watchy();
#elif defined(USE_Nachi)
WatchyNachi watchy = WatchyNachi();
#else
// Default to 7-segment if nothing is defined
Watchy7SEG watchy = Watchy7SEG();
#endif

// SDL2 display class
class WatchySimSDL2 {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* displayTexture;
    SDL_Texture* backgroundTexture;
    uint32_t* displayPixels;
    
    static const int SCALE = 3;
    static const int WINDOW_WIDTH = 676;
    static const int WINDOW_HEIGHT = 676;
    
    // E-ink colors
    static const uint32_t COLOR_WHITE = 0xFFFFFFFF;
    static const uint32_t COLOR_BLACK = 0xFF000000;
    
    // Timer ID
    SDL_TimerID updateTimer;
    
    // Menu state
    bool showMenu = true;
    
public:
    WatchySimSDL2() : window(nullptr), renderer(nullptr), displayTexture(nullptr), 
                      backgroundTexture(nullptr), displayPixels(nullptr), updateTimer(0) {}
    
    bool init() {
        
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
            std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
            return false;
        }
        
        
        // Create window with fixed size (same as Windows version)
        window = SDL_CreateWindow("WatchySim",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            WINDOW_WIDTH, WINDOW_HEIGHT,
            SDL_WINDOW_SHOWN);
            
        if (!window) {
            std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
            return false;
        }
        
        
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (!renderer) {
            std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
            return false;
        }
        
        // Use nearest neighbor scaling for crisp pixels
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
        
        
        // Create display texture
        displayTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING, DISPLAY_WIDTH, DISPLAY_HEIGHT);
            
        if (!displayTexture) {
            std::cerr << "Display texture creation failed: " << SDL_GetError() << std::endl;
            return false;
        }
        
        
        // TODO: Load background image (for now, just use a gray background)
        // In the Windows version, this loads from resources
        // For Linux, we'll need to load from a file
        
        displayPixels = new uint32_t[DISPLAY_WIDTH * DISPLAY_HEIGHT];
        clearDisplay();
        
        
        // Set up timer for updates (1 second interval)
        updateTimer = SDL_AddTimer(1000, timerCallback, this);
        
        // Initialize Dear ImGui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        
        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        
        // Setup Platform/Renderer backends
        ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
        ImGui_ImplSDLRenderer2_Init(renderer);
        
        return true;
    }
    
    void clearDisplay() {
        memset(displayPixels, 0xFF, DISPLAY_WIDTH * DISPLAY_HEIGHT * sizeof(uint32_t));
    }
    
    void drawBackground() {
        // For now, just draw a gray background
        SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
        SDL_RenderClear(renderer);
        
        // TODO: Draw the actual watch background image
        // This would involve loading the background.png and rendering it
    }
    
    void onPaint() {
        // Clear the display buffer
        clearDisplay();
        
        // Create SDL2 graphics context - use display dimensions, not window dimensions!
        SDL2GraphicsContext context(renderer, displayTexture, displayPixels, 
                                   DISPLAY_WIDTH, DISPLAY_HEIGHT);
        
        // Let Watchy draw the watch face
        watchy.showWatchFace(&context);
        
        // Note: context.present() is called which updates the texture
        // updateDisplay() will handle the actual rendering
    }
    
    void drawTestPattern() {
        // Clear to white first
        memset(displayPixels, 0xFF, DISPLAY_WIDTH * DISPLAY_HEIGHT * sizeof(uint32_t));
        
        // Draw a simple test pattern to verify display is working
        for (int y = 0; y < DISPLAY_HEIGHT; y++) {
            for (int x = 0; x < DISPLAY_WIDTH; x++) {
                // Draw a thick border
                if (x < 5 || x >= DISPLAY_WIDTH - 5 || y < 5 || y >= DISPLAY_HEIGHT - 5) {
                    displayPixels[y * DISPLAY_WIDTH + x] = COLOR_BLACK;
                }
                // Draw a grid
                else if (x % 20 == 0 || y % 20 == 0) {
                    displayPixels[y * DISPLAY_WIDTH + x] = COLOR_BLACK;
                }
            }
        }
        
        // Draw text "TEST" in center (rough approximation)
        int cx = DISPLAY_WIDTH / 2;
        int cy = DISPLAY_HEIGHT / 2;
        for (int i = -20; i < 20; i++) {
            for (int j = -5; j < 5; j++) {
                if (cx + i >= 0 && cx + i < DISPLAY_WIDTH && cy + j >= 0 && cy + j < DISPLAY_HEIGHT) {
                    displayPixels[(cy + j) * DISPLAY_WIDTH + (cx + i)] = COLOR_BLACK;
                }
            }
        }
    }
    
    void updateDisplay() {
        // Draw background
        drawBackground();
        
        // Note: The texture should already be updated by context.present() or manually
        // If we're calling this after drawTestPattern, we need to update the texture
        SDL_UpdateTexture(displayTexture, NULL, displayPixels, DISPLAY_WIDTH * sizeof(uint32_t));
        
        // Create destination rectangle centered in window
        SDL_Rect dstRect;
        dstRect.x = (WINDOW_WIDTH - DISPLAY_WIDTH * SCALE) / 2;
        dstRect.y = (WINDOW_HEIGHT - DISPLAY_HEIGHT * SCALE) / 2;
        dstRect.w = DISPLAY_WIDTH * SCALE;
        dstRect.h = DISPLAY_HEIGHT * SCALE;
        
        // Render the display texture
        SDL_RenderCopy(renderer, displayTexture, NULL, &dstRect);
        
        // Present
        SDL_RenderPresent(renderer);
    }
    
    static Uint32 timerCallback(Uint32 interval, void* param) {
        (void)param; // Unused parameter
        
        // Get current time
        time_t curr_time = time(NULL);
        struct tm* tm_local = localtime(&curr_time);
        
        // Convert to Watchy time structure
        struct tm watchy_tm = *tm_local;
        watchy.setTime(watchy_tm);
        
        // Always update display every second
        // This allows seconds display mode to work properly
        SDL_Event event;
        SDL_memset(&event, 0, sizeof(event));
        event.type = SDL_USEREVENT;
        event.user.code = 1; // 1 = timer update
        SDL_PushEvent(&event);
        
        return interval; // Continue timer
    }
    
    void run() {
        bool running = true;
        SDL_Event event;
        
        // Initial paint with a small delay to ensure window is visible
        SDL_Delay(50);  // Give window time to appear
        onPaint();
        updateDisplay();
        SDL_RenderPresent(renderer);  // Force immediate presentation
        
        
        while (running) {
            while (SDL_PollEvent(&event)) {
                // Process ImGui events
                ImGui_ImplSDL2_ProcessEvent(&event);
                
                switch (event.type) {
                    case SDL_QUIT:
                        running = false;
                        break;
                        
                    case SDL_KEYDOWN:
                        // Don't process key events if ImGui wants keyboard input
                        if (!ImGui::GetIO().WantCaptureKeyboard) {
                            handleKeyPress(event.key.keysym.sym);
                        }
                        break;
                        
                    case SDL_USEREVENT:
                        if (event.user.code == 1) {
                            // Timer update - repaint display
                            onPaint();
                            // Don't call updateDisplay here, we'll do it after ImGui
                        }
                        break;
                }
            }
            
            // Start ImGui frame
            ImGui_ImplSDLRenderer2_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();
            
            // Render menu if visible
            if (showMenu) {
                renderMenu();
            }
            
            // Rendering
            ImGui::Render();
            
            // Clear and draw background
            drawBackground();
            
            // Draw the watch display
            SDL_Rect dstRect;
            dstRect.x = (WINDOW_WIDTH - DISPLAY_WIDTH * SCALE) / 2;
            dstRect.y = (WINDOW_HEIGHT - DISPLAY_HEIGHT * SCALE) / 2;
            dstRect.w = DISPLAY_WIDTH * SCALE;
            dstRect.h = DISPLAY_HEIGHT * SCALE;
            SDL_RenderCopy(renderer, displayTexture, NULL, &dstRect);
            
            // Render ImGui on top
            ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
            
            // Present
            SDL_RenderPresent(renderer);
            
            // Small delay to prevent CPU spinning
            SDL_Delay(10);
        }
    }
    
    void handleKeyPress(SDL_Keycode key) {
        switch (key) {
            case SDLK_ESCAPE:
                SDL_Event quit_event;
                quit_event.type = SDL_QUIT;
                SDL_PushEvent(&quit_event);
                break;
                
            // Menu shortcuts (commented out for now)
            /*
            case SDLK_t:
                // Time menu - reset to current time
                watchy.resetTime();
                onPaint();
                updateDisplay();
                break;
                
            case SDLK_b:
                // Battery menu - cycle through battery levels
                static float batteryLevels[] = {0.0f, 3.7f, 3.81f, 3.96f, 4.2f};
                static int currentBatteryIndex = 4;
                currentBatteryIndex = (currentBatteryIndex + 1) % 5;
                watchy.setBatteryVoltage(batteryLevels[currentBatteryIndex]);
                onPaint();
                updateDisplay();
                break;
                
            case SDLK_w:
                // WiFi toggle
                static bool wifiOn = false;
                wifiOn = !wifiOn;
                watchy.setWifi(wifiOn);
                if (wifiOn) {
                    watchy.setWeatherExternal(true);
                    if (watchy.getTemperature() == WATCHY_INTERNAL_TEMP) {
                        watchy.setTemperature(WATCHY_DEFAULT_TEMP);
                    }
                } else {
                    watchy.setWeatherCode(800);
                    watchy.setWeatherExternal(false);
                    watchy.setTemperature(WATCHY_INTERNAL_TEMP);
                }
                onPaint();
                updateDisplay();
                break;
            */
                
            case SDLK_r:
                // Refresh display
                onPaint();
                break;
                
            case SDLK_m:
                // Toggle menu
                showMenu = !showMenu;
                break;
                
            case SDLK_t:
                // Test pattern
                drawTestPattern();
                updateDisplay();
                break;
                
            // Button simulations
            case SDLK_1:
            case SDLK_q:
                watchy.handleButtonPress(1);
                onPaint();
                updateDisplay();
                break;
                
            case SDLK_2:
            case SDLK_w:
                watchy.handleButtonPress(2);
                onPaint();
                updateDisplay();
                break;
                
            case SDLK_3:
            case SDLK_e:
                watchy.handleButtonPress(3);
                onPaint();
                updateDisplay();
                break;
                
            case SDLK_4:
                watchy.handleButtonPress(4);
                onPaint();
                updateDisplay();
                break;
        }
    }
    
    void renderMenu() {
        // Create a dockable menu window
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(300, 600), ImGuiCond_FirstUseEver);
        
        if (ImGui::Begin("WatchySim Control Panel", &showMenu)) {
            // Time Menu
            if (ImGui::CollapsingHeader("Time")) {
                if (ImGui::Button("Current Time")) {
                    time_t curr_time = time(NULL);
                    struct tm* tm_local = localtime(&curr_time);
                    watchy.setTime(*tm_local);
                    onPaint();
                }
                if (ImGui::Button("Short (01:01:01 May 1, 2011)")) {
                    struct tm custom_time = {0};
                    custom_time.tm_year = 111; // 2011-1900
                    custom_time.tm_mon = 4;     // May (0-based)
                    custom_time.tm_mday = 1;
                    custom_time.tm_hour = 1;
                    custom_time.tm_min = 1;
                    custom_time.tm_sec = 1;
                    watchy.setTime(custom_time);
                    onPaint();
                }
                if (ImGui::Button("Long (18:33:01 Sep 31, 2099)")) {
                    struct tm custom_time = {0};
                    custom_time.tm_year = 199; // 2099-1900
                    custom_time.tm_mon = 8;     // September (0-based)
                    custom_time.tm_mday = 31;
                    custom_time.tm_hour = 18;
                    custom_time.tm_min = 33;
                    custom_time.tm_sec = 1;
                    watchy.setTime(custom_time);
                    onPaint();
                }
            }
            
            // Battery Menu
            if (ImGui::CollapsingHeader("Battery")) {
                float voltage = watchy.getBatteryVoltage();
                ImGui::Text("Current: %.2fV", voltage);
                
                if (ImGui::Button("Dead (0.0V)")) {
                    watchy.setBatteryVoltage(0.0f);
                    onPaint();
                }
                ImGui::SameLine();
                if (ImGui::Button("Low (3.7V)")) {
                    watchy.setBatteryVoltage(3.7f);
                    onPaint();
                }
                if (ImGui::Button("Medium (3.81V)")) {
                    watchy.setBatteryVoltage(3.81f);
                    onPaint();
                }
                ImGui::SameLine();
                if (ImGui::Button("High (3.96V)")) {
                    watchy.setBatteryVoltage(3.96f);
                    onPaint();
                }
                if (ImGui::Button("Max (4.2V)")) {
                    watchy.setBatteryVoltage(4.2f);
                    onPaint();
                }
            }
            
            // Bluetooth Menu
            if (ImGui::CollapsingHeader("Bluetooth")) {
                static bool btEnabled = false;
                ImGui::Text("Status: %s", btEnabled ? "Enabled" : "Disabled");
                
                if (ImGui::Button("Toggle Bluetooth")) {
                    btEnabled = !btEnabled;
                    watchy.setBluetooth(btEnabled);
                    onPaint();
                }
            }
            
            // WiFi Menu
            if (ImGui::CollapsingHeader("WiFi")) {
                static bool wifiEnabled = false;
                ImGui::Text("Status: %s", wifiEnabled ? "On" : "Off");
                
                if (ImGui::Button("Toggle WiFi")) {
                    wifiEnabled = !wifiEnabled;
                    watchy.setWifi(wifiEnabled);
                    onPaint();
                }
            }
            
            // Steps Menu
            if (ImGui::CollapsingHeader("Steps")) {
                static int currentSteps = 0;
                ImGui::Text("Current: %d steps", currentSteps);
                
                if (ImGui::Button("None (0)")) {
                    currentSteps = 0;
                    watchy.setSteps(0);
                    onPaint();
                }
                ImGui::SameLine();
                if (ImGui::Button("Lazy (12)")) {
                    currentSteps = 12;
                    watchy.setSteps(12);
                    onPaint();
                }
                if (ImGui::Button("Regular (5,280)")) {
                    currentSteps = 5280;
                    watchy.setSteps(5280);
                    onPaint();
                }
                ImGui::SameLine();
                if (ImGui::Button("Athlete (52,769)")) {
                    currentSteps = 52769;
                    watchy.setSteps(52769);
                    onPaint();
                }
                
                // Custom step input
                static int customSteps = 0;
                ImGui::InputInt("Custom Steps", &customSteps);
                if (ImGui::Button("Set Custom")) {
                    currentSteps = customSteps;
                    watchy.setSteps(customSteps);
                    onPaint();
                }
            }
            
            // Weather Menu
            if (ImGui::CollapsingHeader("Weather")) {
                weatherData weather = watchy.getWeatherData();
                ImGui::Text("Current Code: %d", weather.weatherConditionCode);
                
                if (ImGui::Button("Clear (800)")) {
                    watchy.setWeatherCode(800);
                    onPaint();
                }
                ImGui::SameLine();
                if (ImGui::Button("Few Clouds (801)")) {
                    watchy.setWeatherCode(801);
                    onPaint();
                }
                if (ImGui::Button("Cloudy (802)")) {
                    watchy.setWeatherCode(802);
                    onPaint();
                }
                ImGui::SameLine();
                if (ImGui::Button("Atmosphere (750)")) {
                    watchy.setWeatherCode(750);
                    onPaint();
                }
                if (ImGui::Button("Snow (650)")) {
                    watchy.setWeatherCode(650);
                    onPaint();
                }
                ImGui::SameLine();
                if (ImGui::Button("Rain (550)")) {
                    watchy.setWeatherCode(550);
                    onPaint();
                }
                if (ImGui::Button("Drizzle (350)")) {
                    watchy.setWeatherCode(350);
                    onPaint();
                }
                ImGui::SameLine();
                if (ImGui::Button("Thunderstorm (250)")) {
                    watchy.setWeatherCode(250);
                    onPaint();
                }
            }
            
            // Temperature Menu
            if (ImGui::CollapsingHeader("Temperature")) {
                weatherData weather = watchy.getWeatherData();
                int8_t temp = watchy.getTemperature();
                ImGui::Text("Current: %d%s", temp, weather.isMetric ? "°C" : "°F");
                
                if (ImGui::Button("Celsius")) {
                    watchy.setTemperatureUnitMetric(true);
                    onPaint();
                }
                ImGui::SameLine();
                if (ImGui::Button("Fahrenheit")) {
                    watchy.setTemperatureUnitMetric(false);
                    onPaint();
                }
                
                ImGui::Separator();
                
                if (ImGui::Button("Canada (-45°)")) {
                    watchy.setTemperature(-45);
                    onPaint();
                }
                ImGui::SameLine();
                if (ImGui::Button("Chilly (7°)")) {
                    watchy.setTemperature(7);
                    onPaint();
                }
                if (ImGui::Button("Warm (15°)")) {
                    watchy.setTemperature(15);
                    onPaint();
                }
                ImGui::SameLine();
                if (ImGui::Button("Inferno (40°)")) {
                    watchy.setTemperature(40);
                    onPaint();
                }
                
                // Custom temperature input
                static int customTemp = 20;
                ImGui::InputInt("Custom Temp", &customTemp);
                if (ImGui::Button("Set Custom")) {
                    watchy.setTemperature((int8_t)customTemp);
                    onPaint();
                }
            }
            
            // Tools Menu
            if (ImGui::CollapsingHeader("Tools")) {
                if (ImGui::Button("Screenshot")) {
                    // TODO: Implement screenshot functionality
                    ImGui::Text("Screenshot saved (TODO)");
                }
                
                ImGui::Separator();
                
                if (ImGui::Button("Refresh Display")) {
                    onPaint();
                }
                
                if (ImGui::Button("Test Pattern")) {
                    drawTestPattern();
                }
            }
            
            // About
            if (ImGui::CollapsingHeader("About")) {
                ImGui::Text("WatchySim Linux Port");
                ImGui::Text("Using Dear ImGui for menus");
                ImGui::Separator();
                ImGui::Text("Press 'M' to toggle this menu");
                ImGui::Text("Press 'ESC' to exit");
            }
        }
        ImGui::End();
    }
    
    ~WatchySimSDL2() {
        if (updateTimer) {
            SDL_RemoveTimer(updateTimer);
        }
        
        // Cleanup ImGui
        ImGui_ImplSDLRenderer2_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
        
        if (displayPixels) delete[] displayPixels;
        if (displayTexture) SDL_DestroyTexture(displayTexture);
        if (backgroundTexture) SDL_DestroyTexture(backgroundTexture);
        if (renderer) SDL_DestroyRenderer(renderer);
        if (window) SDL_DestroyWindow(window);
        SDL_Quit();
    }
};

// Helper function for day of week calculation
int getDayOfWeek(int y, int m, int d) {
    static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
    if (m < 3) {
        y -= 1;
    }
    return (y + y / 4 - y / 100 + y / 400 + t[m - 1] + d) % 7;
}

// Main function
int main(int argc, char* argv[]) {
    
    WatchySimSDL2 sim;
    
    
    if (!sim.init()) {
        std::cerr << "[ERROR] Failed to initialize!" << std::endl;
        return 1;
    }
    
    
    
    sim.run();
    
    return 0;
}