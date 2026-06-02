#include <Arduino.h>
#include "VlajkovaBleNode.h" 

VlajkovaBleNode bleNode;

void setup() {
    Serial.begin(115200);
    bleNode.begin();
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

    delay(10); 
}
