/**
 * @brief Data types of osurs
 *
 * All data types used by osurs are declared and defined here.
 *
 * @file types.h
 * @date: 2022-08-22
 * @author: Merlin Unterfinger
 */

#ifndef OSURS_TYPES_H_
#define OSURS_TYPES_H_

#include <osurs/ds.h>
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
struct seat_t;
struct seat_collection_t;

/**
 * @brief A node.
 *
 * A node is a stop location used by different routes.
 */
typedef struct node_t {
    char *id;        /**< Identifier. */
    double x;        /**< X coordinate. */
    double y;        /**< Y coordinate. */
    HashMap *routes; /**< HashMap with routes passing the node. */
} Node;

/**
 * @brief A stop.
 *
 * A stop is a stop location on a trip pointing on a node in the network. Each
 * stop contains information about which stop is next and how long it takes to
 * reach it.
 */
typedef struct stop_t {
    int time_to_next;     /**< Identifier. */
    int arrival_offset;   /**< The offset from the root stop arrival. */
    int departure_offset; /**< The offset from the root stop departure. */
    int *reserved; /**< Array with the number of reservations on the stop for
                      each trip. */
    struct node_t *node; /**< The corresponding node in the network. */
    struct stop_t *prev; /**< The previous stop on the route. */
    struct stop_t *next; /**< The next stop on the route. */
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
    char *id;      /**< Identifier. */
    int departure; /**< Departure time in seconds after midnight of the trip at
                      the first stop of the route. */
    int arrival; /**< Arrival time in seconds after midnight of the trip at the
                    last stop of the route. */
    struct vehicle_t *vehicle; /**< The vehicle used to travel along the route
                                  with this trip / departure. */
    struct trip_t *next;   /**< The next trip starting after the current one. */
    struct route_t *route; /**< The route the trip corresponds to. */
    ArrayList *reservations; /**< Reservation in the network. */
} Trip;

/**
 * @brief A route.
 *
 * A route is a group of trips, which defines the route through the node
 * network using stops. A route stores the order in which the nodes are
 * approached by a vehicle in a chain of stops.
 */
typedef struct route_t {
    char *id; /**< Identifier. */
    struct stop_t
        *root_stop; /**< The first stop (head) of the chain of stops. */
    struct trip_t
        *root_trip; /**< The first trip (head) of the linked list of trips. */
    size_t route_size; /**< Number of stops in the route. */
    size_t trip_size;  /**< Number of trips on the route. */
} Route;

/**
 * @brief A vehicle.
 *
 * A vehicle consists of an identifier and vehicle composition.
 */
typedef struct vehicle_t {
    char *id;                          /**< Identifier. */
    struct composition_t *composition; /**< The composition of the vehicle. */
} Vehicle;

/**
 * @brief A reservation.
 *
 * A reservation is a booked connection. The booked seats have to be smaller or
 * equal to the available seats.
 */
typedef struct reservation_t {
    char id[37]; /**< Automatically generated UUID of the reservation. */
    int seat_count;   /**< Reserved seats. */
    struct seat_t** seat_arr; /**< Seat array */
    int seat_arr_index; /**< The current index of the seat array */
    struct stop_t* orig; /**< The orig stop of the reservation. */
    struct stop_t* dest; /**< The orig stop of the reservation. */
    struct trip_t* trip; /**< The trip on which the reservation is placed. */

} Reservation;

/**
 * @brief A seat
 *
 * A seat contains an array of reservations.
 */
typedef struct seat_t {
    int seat_id;     /**< Seat id. */
    int res_count;   /**< Number of reservations. */
    Reservation** res_arr; /**< Array that contains each reservation. */
} Seat;

/**
 * @brief A composition.
 *
 * A composition reflects a combination of containers / groups with au defined
 * seat capacity.
 */
typedef struct composition_t {
    char *id;       /**< Identifier. */
    int seat_count; /**< The seat capacity of the composition (TODO: Replace
                  with groups in future). */
    Seat **seats;  /**< The seat array */
} Composition;

/**
 * @brief A network.
 *
 * A network consists of nodes where vehicles stop and passengers can get on
 * and off.
 */
typedef struct network_t {
    HashMap *nodes;        /**< Nodes in the network. */
    HashMap *routes;       /**< Routes in the network. */
    HashMap *vehicles;     /**< Vehicles in the network. */
    HashMap *compositions; /**< Compositions in the network. */
} Network;

/**
 * @brief A connection.
 *
 * A connection is a trip between two nodes that is reachable in time.
 */
typedef struct connection_t {
    int departure;       /**< Departure time in seconds. */
    int arrival;         /**< Arrival time in seconds. */
    int available;       /**< Available seats (capacity - reserved). */
    struct stop_t *orig; /**< Origin stop. */
    struct stop_t *dest; /**< Destination stop. */
    struct trip_t
        *trip; /**< The trip on the route on which the connection is. */
    struct connection_t
        *next; /**< If there are more than one result, chain of connections,
                  NULL if at the end of the chain. */
    struct connection_t
        *prev; /**< Previous connection or NULL if at the start of the chain. */
} Connection;

/**
 * @brief A seat collection
 *
 * The seat collection represents multiple seats over which the reservations
 * were distributed/optimized.
 */
typedef struct seat_collection_t {
    Seat **seat_arr; /**< Array that contains all the available seats. */
    int seat_count;  /**< Number of seats in the collection. */
} SeatCollection;

#endif  // OSURS_TYPES_H_
