#pragma once
#include <optional>
#include <sqlite3.h>
#include "Building.h"


int insertBuildingRaw(sqlite3* db, const Building& building);

std::optional<Building> findBuildingRaw(sqlite3* db, int id);

