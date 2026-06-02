#include <cstdint>
#pragma once

struct __attribute__((packed)) CampInfo {
    uint16_t azimuth;
    uint16_t distance;
    uint16_t sign;
};
