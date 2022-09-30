/**
 * @brief Data types of osurs.
 *
 * All data types used by osurs are declared and defined here.
 *
 * @file types.h
 * @date: 2022-08-22
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
    char *id;                /**< Identifier. */
    double x;                /**< X coordinate. */
    double y;                /**< Y coordinate. */
    struct route_t **routes; /**< Array with routes passing the node. */
    size_t route_counter;    /**< The number of routes passing the node. */
    size_t route_size;       /**< The allocated size of the routes array. */
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
    struct vehicle_t *vehicle; /**< The vhicle used to travel along the route
                                  with this trip / departure. */
    struct trip_t *next;   /**< The next trip starting after the current one. */
    struct route_t *route; /**< The route the trip corresponds to. */
    struct reservation_t **reservations; /**< Reservation in the network. */
    size_t reservation_counter; /**< Number of reservations in the network. */
    size_t reservation_size;    /**< Allocated size for reservations. */
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
 * @brief A composition.
 *
 * A composition refelects a combination of containers / groups with au defined
 * seat capactity.
 */
typedef struct composition_t {
    char *id;  /**< Identifier. */
    int seats; /**< The seat capacity of the composition (TODO: Replace with
                  groups in future). */
} Composition;

/**
 * @brief A network.
 *
 * A network consists of nodes where vehicles stop and passengers can get on
 * and off.
 */
typedef struct network_t {
    struct node_t **nodes;       /**< Nodes in the network. */
    size_t node_counter;         /**< Number of nodes in the network. */
    size_t node_size;            /**< Allocated size for nodes. */
    struct route_t **routes;     /**< Routes in the network. */
    size_t route_counter;        /**< Number of routes in the network. */
    size_t route_size;           /**< Allocated size for routes. */
    struct vehicle_t **vehicles; /**< Vehicles in the network. */
    size_t vehicle_counter;      /**< Number of vehicles in the network. */
    size_t vehicle_size;         /**< Allocated size for vehicles. */
    struct composition_t **compositions; /**< Compositions in the network. */
    size_t composition_counter; /**< Number of compositions in the network. */
    size_t composition_size;    /**< Allocated size for compositions. */
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
 * @brief A reservation.
 *
 * A reservation is a booked connection. The booked seats have to be smaller or
 * equal to the avalable seats.
 */
typedef struct reservation_t {
    int seats;           /**< Reserved seats. */
    struct stop_t *orig; /**< The orig stop of the reservation. */
    struct stop_t *dest; /**< The orig stop of the reservation. */
    struct trip_t *trip; /**< The trip on which the reservation is placed. */

} Reservation;

/**
* @brief A seat
*
* A seat contains an array of reservations.
*/
typedef struct seat_t {
	int seat_id;        /**< Seat id */
	int res_count;      /**< Number of reservations */
	int* res_id_arr;    /**< Array that contains each reservation id */
} Seat;

/**
* @brief A seat collection
*
* The seat collection represents multiple seats over which the reservations were distributed/optimized.
*/
typedef struct seat_collection_t {
	Seat** seat_arr;    /**< Array that contains all the available seats. */
	int seat_count;     /**< Number of seats in the collection. */
} Seat_collection;


#endif  // OSURS_TYPES_H_
