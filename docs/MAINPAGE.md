# Optimization of space utilization in reservation systems

An algorithm for optimal distribution of reservations along a trip among available seats.

The **osurs** library contains the following modules with corresponding headers:

- **`ds.h`:** Data structures for efficient data handling.
- **`io.h`:** Input and output of networks and its structures.
- **`network.h`:** Network for reservation optimization.
- **`reserve.h`:** Connection routing, checking seat availability and reservation.
- **`optimize.h`:** Optimizing space utilization in reservation systems.
- **`olal.h`:** Optimization logic abstraction layer.
- **`types.h`:** Data types of osurs.

**Note:** The core functionality of **osurs** is optimizing reservations. There are already powerful routing algorithms for public transport, so the algorithm included here is minimal and only serves to book reservations on already known/found connections to the right segments of the trips (without transfers).
