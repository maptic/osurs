/**
 * @brief Data types of osurs.
 *
 * All data types used by osurs are declared and defined here.
 *
 * @file types.h
 * @date: 2022-07-12
 * @author: Merlin Unterfinger
 */

#ifndef OSURS_TYPES_H_
#define OSURS_TYPES_H_

#include <stdlib.h>

#define MINUTES 60
#define HOURS 3600

// Forward declarations to enable circular dependencies
struct node_t;
struct stop_t;
struct trip_t;
struct route_t;
struct vehicle_t;
struct composition_t;
struct connection_t;
struct reservation_t;
struct network_t;

/**
 * @brief A node.
 *
 * A node is a stop location used by different routes.
 */
typedef struct node_t {
    char *id;
    double x;
    double y;
    struct route_t **routes;
    size_t route_counter;
    size_t route_size;
} Node;

/**
 * @brief A stop.
 *
 * A stop is a stop location on a trip pointing on a node in the network. Each
 * stop contains information about which stop is next and how long it takes to
 * reach it.
 */
typedef struct stop_t {
    int time_to_next;
    int arrival_offset;    // The offset from the root stop arrival.
    int departure_offset;  // The offset from the root stop departure.
    int *reserved;
    struct node_t *node;
    struct stop_t *last;
    struct stop_t *next;
} Stop;

/**
 * @brief A trip.
 *
 * A trip is a sequence of two or more stop nodes that start at specific
 * time on a given route. Trips indicate the departure times at which a vehicle
 * leaves from the route's root stop. Vehicle information such as capacity and
 * reservations are stored at the trip level.
 */
typedef struct trip_t {
    char *id;
    int departure;
    struct vehicle_t *vehicle;
    struct trip_t *next;
    struct route_t *route;
} Trip;

/**
 * @brief A route.
 *
 * A route is a group of trips, which defines the route through the node
 * network using stops. A route stores the order in which the nodes are
 * approached by a vehicle in a chain of stops.
 */
typedef struct route_t {
    char *id;
    struct stop_t *root_stop;
    struct trip_t *root_trip;
} Route;

/**
 * @brief A vehicle.
 *
 * A vehicle consists of an identifier and vehicle composition.
 */
typedef struct vehicle_t {
    char *id;
    struct composition_t *composition;
} Vehicle;

/**
 * @brief A composition.
 *
 * A composition refelects a combination of containers / groups with au defined
 * seat capactity.
 */
typedef struct composition_t {
    char *id;
    int seats;  // replace with groups in future
} Composition;

/**
 * @brief A network.
 *
 * A network consists of nodes where vehicles stop and passengers can get on
 * and off.
 */
typedef struct network_t {
    struct node_t **nodes;
    size_t node_counter;
    size_t node_size;
    struct route_t **routes;
    size_t route_counter;
    size_t route_size;
    struct vehicle_t **vehicles;
    size_t vehicle_counter;
    size_t vehicle_size;
    struct composition_t **compositions;
    size_t composition_counter;
    size_t composition_size;
} Network;

/**
 * @brief A connection.
 *
 * A connection is a trip between two nodes that is reachable in time.
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

/**
 * @brief A reservation.
 *
 * A reservation is a booked connection. The booked seats have to be smaller or
 * equal to the avalable seats.
 */
typedef struct reservation_t {
    int seats;                       // Reserved seats
    struct connection_t connection;  // The booked connection
} Reservation;

#endif  // OSURS_TYPES_H_
