# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

- `io.h`: Importing and exporting networks (network, vehicles and reservations).
  - Importer for MATSim transit schedules.
- `network.h`: Structures to represent a network of a reservation system.
- `optimize.h`: Optimizing space utilization in reservation systems.
- `reserve.h`: Simple routing alogrithm without transfers for checking seat availability in connections.
- `types.h`: Data types of osurs.

### Changed

- Renamed `routing.h` module to `reserve.h` module.
- Changed from capacities on trips to capacities on compositions, which are linked to the trips via vehicles.
- Renamed member variable `last` to `prev` in order to comply with common linked list and chain definitions.
- Change from `time_to_next` to `arrival_offset` and `departure_offset` on stops.
- Added overall arrival times to trips and optimized connection search (arrival at terminal > time of connection departure).
- Changed prints to XML format.

### Fixed

- Memory leaks in `network.h` module.
- Memory leaks in `reserve.h` module.
- Memory leaks in `io.h` module.

## [0.0.1] - 2022-XX-XX
