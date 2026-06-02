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

    void writeCamps(const CampInfo* camps, size_t count) {
        display.clearDisplay();

        const int ROW_HEIGHT = 8;
        const int COMPASS_WIDTH = 51;
        const int COMPASS_CENTER_X = 25; 
        const int LOGO_X = 56;           

        for (size_t i = 0; i < count; ++i) {
            int baseY = i * ROW_HEIGHT;
            int centerY = baseY + (ROW_HEIGHT / 2);

            // Horizon line
            display.drawLine(0, centerY, COMPASS_WIDTH - 1, centerY, SSD1306_WHITE);

            // Center indicator (0 deg)
            display.drawPixel(COMPASS_CENTER_X, centerY - 1, SSD1306_WHITE);
            display.drawPixel(COMPASS_CENTER_X, centerY + 1, SSD1306_WHITE);

            // Normalize azimuth to [-180, 180] relative to current heading
            int offset = camps[i].azimuth % 360;
            if (offset > 180) {
                offset -= 360;
            }
            
            // Map [-180, 180] range to compass UI width
            int campX = COMPASS_CENTER_X + (offset * COMPASS_CENTER_X) / 180;
            
            // Distance indicator: size scales inversely with distance
            uint16_t dist = camps[i].distance;

            if (dist <= 20) {
                // Crosshair for immediate proximity
                display.drawLine(campX - 2, centerY, campX + 2, centerY, SSD1306_WHITE); 
                display.drawLine(campX, centerY - 2, campX, centerY + 2, SSD1306_WHITE); 
            } else if (dist <= 100) {
                display.fillRect(campX - 1, centerY - 2, 3, 5, SSD1306_WHITE);
            } else if (dist <= 300) {
                display.fillRect(campX - 1, centerY - 1, 3, 3, SSD1306_WHITE);
            } else if (dist <= 700) {
                display.drawLine(campX, centerY - 1, campX, centerY + 1, SSD1306_WHITE);
            } else {
                display.drawPixel(campX, centerY, SSD1306_WHITE);
            }

            // Render 3x5 logo, reading bits from LSB
            uint16_t logoBits = camps[i].sign;
            int logoStartY = baseY + 1; 
            
            for (int y = 0; y < 5; ++y) {
                for (int x = 0; x < 3; ++x) {
                    if (logoBits & (1 << (y * 3 + x))) {
                        display.drawPixel(LOGO_X + x, logoStartY + y, SSD1306_WHITE);
                    }
                }
            }
        }

        display.display();
    }
};
