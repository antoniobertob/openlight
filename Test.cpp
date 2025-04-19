#include "Test.hpp"
#include <iostream>
#include <unistd.h>
#include "LedStrip.hpp"
#include <math.h>

// Generate color based on angle (HSV to RGB)
uint32_t hsvToRgb(float hue, float saturation, float value) {
    float c = value * saturation;
    float x = c * (1 - fabs(fmod(hue / 60.0, 2) - 1));
    float m = value - c;

    float r, g, b;
    if (hue < 60) { r = c; g = x; b = 0; }
    else if (hue < 120) { r = x; g = c; b = 0; }
    else if (hue < 180) { r = 0; g = c; b = x; }
    else if (hue < 240) { r = 0; g = x; b = c; }
    else if (hue < 300) { r = x; g = 0; b = c; }
    else { r = c; g = 0; b = x; }

    uint8_t red = static_cast<uint8_t>((r + m) * 255);
    uint8_t green = static_cast<uint8_t>((g + m) * 255);
    uint8_t blue = static_cast<uint8_t>((b + m) * 255);

    return (red << 16) | (green << 8) | blue;
}

void test(LedStrip& strip, int nLeds) {
    std::cout << "[INFO] Running Rainbow test..." << std::endl;

    int numRounds = 8;

    std::vector<uint32_t> rainbowColors(nLeds);
    for (int i = 0; i < nLeds; ++i) {
        float hue = (360.0 / nLeds) * i;
        rainbowColors[i] = hsvToRgb(hue, 1.0, 1.0);
    }

    // Move rainbow along LED strip
    for (int round = 1; round <= numRounds; ++round) {
        int delay = 100000 / 8; // Increase speed in each cycle
        for (int offset = 0; offset < nLeds; ++offset) {
            std::vector<uint32_t> shiftedColors(nLeds);
            for (int i = 0; i < nLeds; ++i) {
                shiftedColors[i] = rainbowColors[(i + offset) % nLeds];
            }
            strip.turnOnLeds(shiftedColors);
            usleep(delay);
        }
    }

    // Reduce brightness
    for (int brightness = 255; brightness >= 0; brightness -= 5) {
        std::vector<uint32_t> dimmedColors(nLeds);
        for (int i = 0; i < nLeds; ++i) {
            uint8_t r = (rainbowColors[i] >> 16) & 0xFF;
            uint8_t g = (rainbowColors[i] >> 8) & 0xFF;
            uint8_t b = rainbowColors[i] & 0xFF;

            dimmedColors[i] = strip.defineColor(
                (r * brightness) / 255,
                (g * brightness) / 255,
                (b * brightness) / 255
            );
        }
        strip.turnOnLeds(dimmedColors);
        usleep(5000);
    }

    std::cout << "[INFO] Test completed" << std::endl;
}
