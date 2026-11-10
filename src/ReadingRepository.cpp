#include "ReadingRepository.h"


ReadingRepository::ReadingRepository(Storage& storage) : storage(storage) {}

int ReadingRepository::add(const MeterReading& reading) { return storage.insert(reading); }

std::vector<MeterReading> ReadingRepository::findAll() { return storage.get_all<MeterReading>(); }

void ReadingRepository::remove(int id) { storage.remove<MeterReading>(id); }
