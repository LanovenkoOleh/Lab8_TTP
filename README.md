# Lab 8 — Meter readings ORM (Variant 13)

C++20 console project: Building (1) -> MeterReading (N), SQLite, sqlite_orm, Repository.

## Build (Windows, Visual Studio)

Install Visual Studio C++ workload, CMake, Ninja and vcpkg. Set `VCPKG_ROOT` environment variable to your vcpkg installation folder (the folder containing `scripts/buildsystems/vcpkg.cmake`). Open this folder in Visual Studio as a CMake project. Select the `default` preset, configure, build and run `MeterReadingsORM`.

Alternatively, from a Developer PowerShell with vcpkg configured:

```powershell
cmake --preset default
cmake --build --preset default
.\build\MeterReadingsORM.exe
```

If your vcpkg requires a manifest baseline, add `builtin-baseline` to `vcpkg.json` using the 40-character commit ID of your installed vcpkg registry (`git -C "$env:VCPKG_ROOT" rev-parse HEAD`).

The database `meters.db` is created in the executable's **working directory**. The demo clears and reseeds its two tables on each run (5 buildings, 15 readings), then exercises both APIs, CRUD, filtering, sorting, SUM, JOIN, commit and rollback. **Do not use the demo with production data**: it resets these tables.

The C API is used only for the comparison. The rest uses sqlite_orm. The failed transaction inserts a Building, then attempts a MeterReading with an invalid foreign key. The transaction must roll back both changes.

Note: The source code was prepared against the documented sqlite_orm API. Build validation with the vcpkg dependencies requires a configured local Windows toolchain.
