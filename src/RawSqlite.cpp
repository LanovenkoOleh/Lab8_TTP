#include "RawSqlite.h"
#include <stdexcept>
#include <string>

int insertBuildingRaw(sqlite3* db, const Building& building) {

    const char* sql = "INSERT INTO Building(address,district,apartments) VALUES(?,?,?)";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)

        throw std::runtime_error(sqlite3_errmsg(db));

    try {
        if (sqlite3_bind_text(stmt, 1, building.address.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK ||
            sqlite3_bind_text(stmt, 2, building.district.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK ||
            sqlite3_bind_int(stmt, 3, building.apartments) != SQLITE_OK ||
            sqlite3_step(stmt) != SQLITE_DONE)

            throw std::runtime_error(sqlite3_errmsg(db));

        int id = static_cast<int>(sqlite3_last_insert_rowid(db));

        sqlite3_finalize(stmt);

        return id;

    } catch (...) { sqlite3_finalize(stmt); throw; 
    }
}

std::optional<Building> findBuildingRaw(sqlite3* db, int id) {

    const char* sql = "SELECT id,address,district,apartments FROM Building WHERE id=?";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)

        throw std::runtime_error(sqlite3_errmsg(db));

    try {
        if (sqlite3_bind_int(stmt, 1, id) != SQLITE_OK)

            throw std::runtime_error(sqlite3_errmsg(db));

        int rc = sqlite3_step(stmt);

        if (rc == SQLITE_DONE) { sqlite3_finalize(stmt); return std::nullopt;
        }
        if (rc != SQLITE_ROW) throw std::runtime_error(sqlite3_errmsg(db));

        Building b;

        b.id = sqlite3_column_int(stmt, 0);

        b.address = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));

        b.district = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));

        b.apartments = sqlite3_column_int(stmt, 3);

        sqlite3_finalize(stmt);

        return b;

    } catch (...) { sqlite3_finalize(stmt); throw; 
    }
}
