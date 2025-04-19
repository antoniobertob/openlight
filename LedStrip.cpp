// LedStrip.cpp
#include "LedStrip.hpp"
#include <stdexcept>
#include <iostream>

#define GPIO_PIN 18           // GPIO PIN for data
#define DMA_CHANNEL 10        // DMA channel
#define TARGET_FREQ WS2811_TARGET_FREQ  // PWM frequency


LedStrip::LedStrip(int ledCount) : ledCount(ledCount), isInitialized(false) {
    ledConfig.freq = TARGET_FREQ;
    ledConfig.dmanum = DMA_CHANNEL;

    // Enable channel 0
    ledConfig.channel[0].gpionum = GPIO_PIN;
    ledConfig.channel[0].count = ledCount;
    ledConfig.channel[0].invert = 0;         // Not inverted signal
    ledConfig.channel[0].brightness = 50;   // Brightness
    ledConfig.channel[0].strip_type = WS2811_STRIP_GRB; // LED type (GRB)

    // Disable channel 1
    ledConfig.channel[1].gpionum = 0;
    ledConfig.channel[1].count = 0;
    ledConfig.channel[1].invert = 0;
    ledConfig.channel[1].brightness = 0;
    ledConfig.channel[1].strip_type = 0;

    if (ws2811_init(&ledConfig) == WS2811_SUCCESS) {
        isInitialized = true;
    } else {
        throw std::runtime_error("Error initializing LED strip");
    }
}

LedStrip::~LedStrip() {
    if (isInitialized) {
        ws2811_fini(&ledConfig);
        isInitialized = false;
    }
}

void LedStrip::testLeds() {
    for (int i = 0; i < ledCount; ++i) {
        ledConfig.channel[0].leds[i] = defineColor(255, 0, 0);
    }
    ws2811_render(&ledConfig);
}

void LedStrip::turnOnLeds(const std::vector<uint32_t>& pixelColors) {
    for (size_t i = 0; i < pixelColors.size() && i < static_cast<size_t>(ledCount); ++i) {
        ledConfig.channel[0].leds[i] = pixelColors[i];
    }
    if (ws2811_render(&ledConfig) != WS2811_SUCCESS) {
        throw std::runtime_error("Error updating LEDs");
    }
}

void LedStrip::turnOffLeds() {
    for (int i = 0; i < ledCount; ++i) {
        ledConfig.channel[0].leds[i] = 0;
    }
    if (ws2811_render(&ledConfig) != WS2811_SUCCESS) {
        throw std::runtime_error("Error turning off the LEDs");
    }
}

uint32_t LedStrip::defineColor(int r, int g, int b) {
    return (r << 16) | (g << 8) | b;
}
