#pragma once

#include <Arduino.h>
#include <NimBLEDevice.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

#define MAX_CAMPS 20 

#include "CampInfo.h"

class VlajkovaBleNode : public NimBLECharacteristicCallbacks {
private:
    NimBLEServer* pServer = nullptr;
    NimBLECharacteristic* pCharacteristic = nullptr;
    
    CampInfo camps[MAX_CAMPS];
    size_t campCount = 0;

    void onWrite(NimBLECharacteristic* pChar, NimBLEConnInfo& connInfo) override {
        // Incoming data format: 16b azimuth in degrees, 16b distance in meters, 16b logo (3×15), Little-Endian, example 5A009600AAAA 90° 150m
        std::string value = pChar->getValue();
        size_t len = value.length();
        size_t structSize = sizeof(CampInfo);
        
        // Process only if length is an exact multiple of the struct size
        if (len > 0 && len % structSize == 0) {
            size_t receivedCount = len / structSize;
            
            if (receivedCount > MAX_CAMPS) {
                receivedCount = MAX_CAMPS;
            }
            
            // Overwrite the state array with the freshest data
            memcpy(camps, value.data(), receivedCount * structSize);
            campCount = receivedCount;
        }
    }

public:
    void begin() {
        NimBLEDevice::init("Vlajkova");
        pServer = NimBLEDevice::createServer();
        NimBLEService* pService = pServer->createService(SERVICE_UUID);
        
        pCharacteristic = pService->createCharacteristic(
            CHARACTERISTIC_UUID,
            NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::WRITE_NR
        );
        
        pCharacteristic->setCallbacks(this);
        pServer->start();
        
        NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
        pAdvertising->addServiceUUID(SERVICE_UUID);

        NimBLEAdvertisementData scanResponseData;
        scanResponseData.setName("Vlajkova");
        pAdvertising->setScanResponseData(scanResponseData);

        pAdvertising->start();
    }

    // Return read-only pointer to the current state of camps
    const CampInfo* getCamps() const {
        return camps;
    }

    // Return current number of active camps
    size_t getCampCount() const {
        return campCount;
    }
};
