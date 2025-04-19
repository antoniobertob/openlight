// LedStrip.hpp
#ifndef LEDSTRIP_HPP
#define LEDSTRIP_HPP

#include <cstdint>
#include <vector>
#include <ws2811.h>


class LedStrip {
public:
    explicit LedStrip(int ledCount);
    ~LedStrip();
    void turnOnLeds(const std::vector<uint32_t>& pixelColors);
    void turnOffLeds();
    uint32_t defineColor(int r, int g, int b);

    void testLeds();

private:
    int ledCount;
    ws2811_t ledConfig = {};
    bool isInitialized = false;
};

#endif // LEDSTRIP_HPP
