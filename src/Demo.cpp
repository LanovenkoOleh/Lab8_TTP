#include "Demo.h"
#include "BuildingRepository.h"
#include "ReadingRepository.h"
#include "RawSqlite.h"
#include <iostream>
#include <stdexcept>
#include <string>

void printBuilding(const Building& b) {
    std::cout << "ID: " << b.id << ", address: " << b.address
        << ", district: " << b.district << ", apartments: " << b.apartments << '\n';
}

void seedData(IBuildingRepository& buildings, ReadingRepository& readings) {
    const Building data[] = {
        { 0, "Khreshchatyk 10", "Center", 40 },
        { 0, "Peremohy 15", "West", 65 },
        { 0, "Lisova 7", "East", 30 },
        { 0, "Shevchenka 22", "Center", 80 },
        { 0, "Nauky 4", "South", 50 }
    };
    const char* types[] = { "Water", "Electricity", "Gas" };

    for (int i = 0; i < 5; ++i) {
        int id = buildings.add(data[i]);
        for (int j = 0; j < 3; ++j)
            readings.add({ 0, id, types[j], "2026-11-01", (i + 1) * 100.0 + j * 25.5 });
    }
}

void runDemo() {
    const std::string filename = "meters.db";
    auto storage = createStorage(filename);

    storage.on_open = [](sqlite3* db) {
        sqlite3_exec(db, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);
        };

    storage.sync_schema();
    storage.open_forever();

    BuildingRepository concreteRepository(storage);
    IBuildingRepository& buildings = concreteRepository;
    ReadingRepository readings(storage);

    storage.remove_all<MeterReading>();
    storage.remove_all<Building>();
    seedData(buildings, readings);

    std::cout << "SQLite version: " << sqlite3_libversion() << '\n';
    std::cout << "Seed data: " << buildings.findAll().size() << " buildings, "
        << readings.findAll().size() << " readings\n";

    std::cout << "\n1. SQLite C API vs ORM\n";
    sqlite3* db = nullptr;
    if (sqlite3_open(filename.c_str(), &db) != SQLITE_OK) {
        std::string error = db ? sqlite3_errmsg(db) : "Cannot open database";
        sqlite3_close(db);
        throw std::runtime_error(error);
    }

    try {
        int rawId = insertBuildingRaw(db, { 0, "Raw API 1", "Test", 10 });
        auto raw = findBuildingRaw(db, rawId);
        if (raw) { std::cout << "SQLite C API: "; printBuilding(*raw); }

        int ormId = buildings.add({ 0, "ORM API 1", "Test", 20 });
        auto orm = buildings.findById(ormId);
        if (orm) { std::cout << "sqlite_orm: "; printBuilding(*orm); }

        buildings.remove(rawId);
        buildings.remove(ormId);
    }
    catch (...) {
        sqlite3_close(db);
        throw;
    }
    sqlite3_close(db);

    std::cout << "\n2. Repository CRUD\n";
    int id = buildings.add({ 0, "Temporary 3", "North", 12 });
    auto found = buildings.findById(id);
    if (found) {
        printBuilding(*found);
        found->apartments = 18;
        buildings.update(*found);
    }

    found = buildings.findById(id);
    if (found) { std::cout << "Updated: "; printBuilding(*found); }
    buildings.remove(id);
    std::cout << "Deleted: " << (!buildings.findById(id) ? "yes" : "no") << '\n';

    using namespace sqlite_orm;

    std::cout << "\n3. Filter: Center buildings\n";
    for (const auto& b : storage.get_all<Building>(where(c(&Building::district) == "Center")))
        printBuilding(b);

    std::cout << "\n4. Sort by apartments (descending), limit 3\n";
    for (const auto& b : storage.get_all<Building>(order_by(&Building::apartments).desc(), limit(3)))
        printBuilding(b);

    std::cout << "\n5. Aggregation: SUM of reading values\n";
    auto total = storage.sum(&MeterReading::value);
    std::cout << "Total readings: " << (total ? *total : 0.0) << '\n';

    std::cout << "\n6. INNER JOIN (fields from both tables)\n";
    auto joined = storage.select(
        columns(&Building::address, &Building::district, &MeterReading::meterType, &MeterReading::value),
        inner_join<MeterReading>(on(c(&Building::id) == &MeterReading::buildingId)),
        limit(8)
    );

    for (const auto& row : joined)
        std::cout << std::get<0>(row) << " | " << std::get<1>(row) << " | "
        << std::get<2>(row) << " | " << std::get<3>(row) << '\n';

    std::cout << "\n7. Successful transaction\n";
    auto beforeBuildings = storage.count<Building>();
    auto beforeReadings = storage.count<MeterReading>();

    bool success = storage.transaction([&] {
        int newId = buildings.add({ 0, "New building", "North", 24 });
        readings.add({ 0, newId, "Water", "2026-11-08", 135.0 });
        return true;
        });

    std::cout << "Committed: " << success << ", buildings: " << beforeBuildings << " -> "
        << storage.count<Building>() << ", readings: " << beforeReadings << " -> "
        << storage.count<MeterReading>() << '\n';

    std::cout << "\n8. Failed transaction and rollback\n";
    beforeBuildings = storage.count<Building>();
    beforeReadings = storage.count<MeterReading>();

    try {
        storage.transaction([&] {
            buildings.add({ 0, "Rollback building", "West", 16 });
            readings.add({ 0, -999, "Gas", "2026-11-08", 250.0 });
            return true;
            });
    }
    catch (const std::exception& ex) {
        std::cout << "Expected foreign key error: " << ex.what() << '\n';
    }

    std::cout << "Buildings: " << beforeBuildings << " -> " << storage.count<Building>()
        << ", readings: " << beforeReadings << " -> " << storage.count<MeterReading>() << '\n';
    std::cout << "Rollback verified: "
        << ((beforeBuildings == storage.count<Building>() &&
            beforeReadings == storage.count<MeterReading>()) ? "yes" : "no") << '\n';



    std::cout << "\n=== Operations without transaction ===\n";

    auto buildingsBefore = storage.count<Building>();
    auto readingsBefore = storage.count<MeterReading>();

    std::cout << "Buildings before: " << buildingsBefore << "\n";
    std::cout << "Readings before: " << readingsBefore << "\n";

    try {
        int newId = buildings.add(
            { 0, "No transaction building", "North", 25 }
        );

        std::cout << "Building added, ID: " << newId << "\n";

        readings.add(
            { 0, -999, "Water", "2026-11-10", 150.0 }
        );

    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }

    auto buildingsAfter = storage.count<Building>();
    auto readingsAfter = storage.count<MeterReading>();

    std::cout << "Buildings after: " << buildingsAfter << "\n";
    std::cout << "Readings after: " << readingsAfter << "\n";

    std::cout << "Building difference: "
        << buildingsAfter - buildingsBefore << "\n";

    std::cout << "Reading difference: "
        << readingsAfter - readingsBefore << "\n";
}