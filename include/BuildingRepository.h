#pragma once
#include <optional>
#include <vector>
#include "Database.h"

class IBuildingRepository {

public:
    virtual ~IBuildingRepository() = default;

    virtual int add(const Building& building) = 0;

    virtual std::optional<Building> findById(int id) = 0;

    virtual std::vector<Building> findAll() = 0;

    virtual void update(const Building& building) = 0;

    virtual void remove(int id) = 0;
};

class BuildingRepository : public IBuildingRepository {

    Storage& storage;

public:

    explicit BuildingRepository(Storage& storage);

    int add(const Building& building) override;

    std::optional<Building> findById(int id) override;

    std::vector<Building> findAll() override;

    void update(const Building& building) override;

    void remove(int id) override;

};
