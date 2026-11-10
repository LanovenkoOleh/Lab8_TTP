cmake_minimum_required(VERSION 3.21)

project(MeterReadingsORM LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SqliteOrm CONFIG REQUIRED)

find_package(unofficial-sqlite3 CONFIG REQUIRED)

add_executable(MeterReadingsORM
    src/main.cpp
    src/Demo.cpp
    src/RawSqlite.cpp
    src/BuildingRepository.cpp
    src/ReadingRepository.cpp)

target_include_directories(MeterReadingsORM PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(MeterReadingsORM PRIVATE sqlite_orm::sqlite_orm unofficial::sqlite3::sqlite3)
