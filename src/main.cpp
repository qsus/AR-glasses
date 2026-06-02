#include <Arduino.h>
#include "VlajkovaBleNode.h" 
#include "CampDisplay.h"

// OLED pins
constexpr int8_t OLED_CS    = D7;
constexpr int8_t OLED_DC    = D9;
constexpr int8_t OLED_RESET = D0;

// SPI pins (HW)
constexpr int8_t SCK_P   = D8;
constexpr int8_t MOSI_P  = D10;

VlajkovaBleNode bleNode;
CampDisplay oled(OLED_DC, OLED_RESET, OLED_CS);

void setup() {
    Serial.begin(115200);
    bleNode.begin();
    
    SPI.begin(SCK_P, -1, MOSI_P, -1); // For now we use D9 as DC, as we don't use MISO
    oled.begin();
}

void loop() {
    // Read camps
    size_t count = bleNode.getCampCount();
    const CampInfo* activeCamps = bleNode.getCamps();
    for (size_t i = 0; i < count; i++) { // For each camp
        uint16_t azimuth = activeCamps[i].azimuth;
		uint16_t distance = activeCamps[i].distance;
        Serial.print(azimuth);
        Serial.print("° ");
        Serial.print(distance);
        Serial.println("m");
    }
	Serial.println();

    if (count == 0) { // print example data
        count = 4;
        static CampInfo exampleCamps[4] = {
            {90, 100, 0b101010101010101},
            {180, 50, 0b111000111000111},
            {180, 10, 0b111000111000111},
            {270, 450, 0b111111111111111}
        };
        activeCamps = exampleCamps;
    }

    oled.writeCamps(activeCamps, count);

    delay(30); 
}
