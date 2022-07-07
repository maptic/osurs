/*
 * Network routing algorithm
 *
 * Simple routing algorithm without transfers for checking seat availability in
 * connections. There are already powerful routing algorithms for public
 * transport, so the algorithm included here is minimal and only serves to book
 * reservations on already known/found connections to the right segments
 * of the trips (without transfers).
 *
 * Author:  Merlin Unterfinger
 */
#ifndef ROUTING_H_
#define ROUTING_H_

#include <stdio.h>

#include "network.h"
/*
 * Connection
 *
 * A connection is a route between two nodes with seats.
 */
typedef struct connection_t {
    int departure;       // Departure time in seconds
    int arrival;         // Arrival time in seconds
    int available;       // Available seats (capacity - reserved)
    struct stop_t orig;  // Origin stop
    struct stop_t dest;  // Destination stop
    struct trip_t trip;  // The trip on the route on which the connection is
    struct connection_t *next;  // If there are more than one result, chain of
                                // connections, NULL if at the end of the chain.
} Connection;

/*
 * Reservation
 *
 * A reservation is a booked connection. The booked seats have to be smaller or
 * equal to the avalable seats.
 */
typedef struct reservation_t {
    int seats;                       // Reserved seats
    struct connection_t connection;  // The booked connection
} Reservation;

Connection *find_connection(Node *orig, Node *dest, int departure);

#endif  // ROUTING_H_
