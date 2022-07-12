/*
 * Network routing algorithm and reservation
 *
 * Simple routing algorithm without transfers for checking seat availability in
 * connections. There are already powerful routing algorithms for public
 * transport, so the algorithm included here is minimal and only serves to book
 * reservations on already known/found connections to the right segments
 * of the trips (without transfers).
 *
 * Author:  Merlin Unterfinger
 */
#ifndef OSURS_RESERVE_H_
#define OSURS_RESERVE_H_

#include <limits.h>
#include <osurs/network.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Connection
 *
 * A connection is a route between two nodes with seats.
 */
typedef struct connection_t {
    int departure;        // Departure time in seconds
    int arrival;          // Arrival time in seconds
    int available;        // Available seats (capacity - reserved)
    struct stop_t *orig;  // Origin stop
    struct stop_t *dest;  // Destination stop
    struct trip_t *trip;  // The trip on the route on which the connection is
    struct connection_t *next;  // If there are more than one result, chain of
                                // connections, NULL if at the end of the chain.
    struct connection_t
        *last;  // Last connection or NULL if at the start of the chain.
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

/*
 * Create connection between nodes.
 *
 * Searches connections between nodes considering departure times.
 *
 * Returns a connection chain or NULL if no connection was found.
 */
Connection *new_connection(const Node *orig, const Node *dest, int departure);

// Reservation

/*
 * Check connection
 *
 * Check a connection if the desired number of seats is available.
 * This is important since reservations can change or avoid double booking of a
 * serached connection. Note: This function is called internally by reserve().
 *
 * Returns 0 if failure and 1 if success.
 */
int check(Connection *connection, int seats, int *trip_count);

/*
 * Reserve connection
 *
 * Books a connection on the network, if the desired seats are available. If
 * enough seats are available the reservation counts of the corresponding trip
 * on the stops of the routes are increased.
 *
 * Returns 0 if failure and 1 if success.
 */
int reserve(Connection *connection, int seats);

// Destructor-like methods

void delete_connection(Connection *connection);

// Print helpers

void print_connection(Connection *connection);

#endif  // OSURS_RESERVE_H_