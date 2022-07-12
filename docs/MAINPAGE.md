# Optimization of space utilization in reservation systems

An algorithm for optimal distribution of reservations along a trip among available seats.

The **osurs** library contains the following modules with corresponding headers:

- **io:** Input and output of networks and its strutures.
- **network:** Network for reservation optmization.
- **optimize:** Optimizing space utilization in reservation systems.
- **reserve:** Connection routing, checking seat availability and reservation.
- **types:** Data types of osurs.

**Note:** The core functionality of **osurs** is in the `optimize.h` module. There are already powerful routing algorithms for public transport, so the algorithm included here is minimal and only serves to book reservations on already known/found connections to the right segments of the trips (without transfers).
