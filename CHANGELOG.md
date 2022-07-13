# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

- `io.h`: Importing and exporting networks (network, vehicles and reservations).
- `network.h`: Structures to represent a network of a reservation system.
- `optimize.h`: Optimizing space utilization in reservation systems.
- `reserve.h`: Simple routing alogrithm without transfers for checking seat availability in connections.
- `types.h`: Data types of osurs.

### Changed

- Renamed `routing.h` module to `reserve.h` module.

### Fixed

- Memory leaks in `network.h` module.
- Memory leaks in `reserve.h` module.

## [0.0.1] - 2022-XX-XX