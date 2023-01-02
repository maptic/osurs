/**
 * @brief Connection routing, checking seat availability and reservation
 *
 * Simple connection routing algorithm without transfers. After checking seat
 * availability in connections reservations can be booked on the network.
 *
 * @note There are already powerful routing algorithms for public transport, so
 * the algorithm included here is minimal and only serves to book reservations
 * on already known/found connections to the right segments of the trips
 * (without transfers).
 *
 * @file reserve.h
 * @date: 2022-07-12
 * @author: Merlin Unterfinger
 */

#ifndef OSURS_RESERVE_H_
#define OSURS_RESERVE_H_

#include "osurs/network.h"

/**
 * @brief Create connection between nodes.
 *
 * Searches connections between nodes on the network considering a departure
 * time. If more than one connection is possible between to nodes on the
 * network, a connection chain is created. In a connection chain, the .last
 * property of the connection structure points to the last connection or NULL if
 * it is the root of the chain. Identically, the .next property points to the
 * next connection or to NULL if it is the end of the chain.
 *
 * @note Queried connections are not stored on the network and must be released
 * individually to prevent a memory leak (delete_connection(Connection*)).
 *
 * @param orig Origin node in network for connection.
 * @param dest Destination node in network for connection.
 * @param time The departure time in seconds after midnight (00:00:00).
 * @return Returns a pointer to a connection chain or NULL if no connection was
 * found.
 */
Connection *new_connection(const Node *orig, const Node *dest, int time);

/**
 * @brief Check if seats are available in connection.
 *
 * Check a connection if the desired number of seats is available.
 * This is important since reservations can change or avoid double booking of a
 * searched connection.
 *
 * @note This function is called internally by new_reservation(Connection).
 *
 * @param connection The queried connection to check.
 * @param seats The number of seats to check.
 * @param trip_count The trip number on the route, needed for getting the
 * reserved count.
 * @return Returns 0 if failure and 1 if success.
 */
int check_connection(Connection *connection, int seats, int *trip_count);

/**
 * @brief Selects the best connection.
 *
 * Chooses the best connection based on arrival time and seat availability.
 *
 * @param connection Connection results from new_connection().
 * @param seats The number of seats that should be available on the connection.
 * @return The shifted pointer to the best connection (Connection*) or NULL if
 * no suitable connection is found.
 */
Connection *select_connection(Connection *connection, int seats);

/*
 * Reserve connection
 *
 * Books a connection on the network, if the desired seats are available. If
 * enough seats are available the reservation counts of the corresponding trip
 * on the stops of the routes are increased.
 *
 * Returns 0 if failure and 1 if success.
 */

/**
 * @brief Create a new reservation.
 *
 * Books a connection on the network, if the desired seats are available. If
 * enough seats are available the reservation counts of the corresponding trip
 * on the stops of the routes are increased and a new reservation is allocated
 * and connected to the network.
 *
 * @param connection The connection to reserve.
 * @param seat_count The number of seats to reserve.
 * @param id The UUID of the reservation or NULL to generate a new one.
 * @return Returns a reservation or null if no seats are available.
 */
Reservation *new_reservation(Connection *connection, int seat_count, char *id);

// Destructor-like methods

/**
 * @brief Delete a connection
 *
 * Frees the memory from a connection chain on the heap.
 *
 * @note This function has to be called for every queried connection to prevent
 * memory leaks.
 *
 * @param connection The connection chain to delete.
 */
void delete_connection(Connection *connection);

#endif  // OSURS_RESERVE_H_