#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "CampInfo.h"

class CampDisplay {
private:
    Adafruit_SSD1306 display;

public:
    CampDisplay(int8_t dc, int8_t reset, int8_t cs, uint8_t width = 128, uint8_t height = 64) 
        : display(width, height, &SPI, dc, reset, cs) {}

    bool begin() {
        if (!display.begin(SSD1306_SWITCHCAPVCC)) {
            return false;
        }
        display.clearDisplay();
        display.setTextColor(SSD1306_WHITE);
        display.display();
        return true;
    }

    // Print camp data
    void writeCamps(const CampInfo* camps, size_t count) {
        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(0, 0);

        for (size_t i = 0; i < count; ++i) {
            // Y-cursor advances automatically with println/printf
            display.printf(
				"%03u° %um\n", // %03u for zero-padded 3-digit azimuth, %u for distance in meters
                camps[i].azimuth, 
                camps[i].distance
			);
        }

        display.display();
    }
};
