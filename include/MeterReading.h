#pragma once
#include <string>

struct MeterReading {

    int id = 0;

    int buildingId = 0;

    std::string meterType;

    std::string readingDate;

    double value = 0.0;

};
