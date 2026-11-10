#include "BuildingRepository.h"
BuildingRepository::BuildingRepository(Storage& storage) : storage(storage) {}

int BuildingRepository::add(const Building& building) { return storage.insert(building); }

std::optional<Building> BuildingRepository::findById(int id) { return storage.get_optional<Building>(id); }

std::vector<Building> BuildingRepository::findAll() { return storage.get_all<Building>(); }

void BuildingRepository::update(const Building& building) { storage.update(building); }

void BuildingRepository::remove(int id) { storage.remove<Building>(id); }
