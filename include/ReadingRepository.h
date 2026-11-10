#pragma once
#include "Database.h"
#include <vector>


class ReadingRepository {

    Storage& storage;

public:

    explicit ReadingRepository(Storage& storage);

    int add(const MeterReading& reading);

    std::vector<MeterReading> findAll();

    void remove(int id);
};
