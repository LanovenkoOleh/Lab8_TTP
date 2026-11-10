#pragma once
#include <string>
#include <sqlite_orm/sqlite_orm.h>
#include "Building.h"
#include "MeterReading.h"


inline auto createStorage(const std::string& filename) {

    using namespace sqlite_orm;

    return make_storage(filename,

        make_table("Building",
            make_column("id", &Building::id, primary_key().autoincrement()),
            make_column("address", &Building::address, not_null()),
            make_column("district", &Building::district, not_null()),
            make_column("apartments", &Building::apartments, not_null())),

        make_table("MeterReading",
            make_column("id", &MeterReading::id, primary_key().autoincrement()),
            make_column("building_id", &MeterReading::buildingId, not_null()),
            make_column("meter_type", &MeterReading::meterType, not_null()),
            make_column("reading_date", &MeterReading::readingDate, not_null()),
            make_column("value", &MeterReading::value, not_null()),

            foreign_key(&MeterReading::buildingId).references(&Building::id)));
}

using Storage = decltype(createStorage(""));
