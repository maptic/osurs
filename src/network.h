/*
 * Network for reservation optmization
 *
 * The network consists of nodes with a name and coordinates. A route is defined
 * by stops and trips. Each stop is associated to a node of the network and
 * knows the previous and next stop if there is one. A route can have several
 * trips, which are defined by departure times and their seat capacity. Each
 * trip knows the next departing trip, if there is one.
 *
 * Author:  Merlin Unterfinger
 */

#ifndef NETWORK_H_
#define NETWORK_H_

#include <stdio.h>
#include <stdlib.h>

#define INIT_SIZE 100
#define MINUTES 60
#define HOURS 3600

// Forward declarations to enable circular dependencies
struct note_t;
struct stop_t;
struct trip_t;
struct route_t;
struct network_t;

/*
 * Node
 * A node is a stop location used by different routes.
 */
typedef struct node_t {
    double x;
    double y;
    char *name;
    struct route_t **routes;
    size_t route_counter;
    size_t route_size;
} Node;

/*
 * Stop
 * A stop is a stop location on a trip pointing on a node in the network.
 */
typedef struct stop_t {
    int time_to_next;
    struct node_t *node;
    struct stop_t *last;
    struct stop_t *next;
} Stop;

/*
 * Trip
 * A trip is a sequence of two or more stop nodes that start at specific time on
 * a given route.
 */
typedef struct trip_t {
    int departure;
    int capacity;
    struct trip_t *next;
    struct route_t *route;
} Trip;

/*
 * Route
 * A route is a group of trips, which defines the route through the node network
 * using stops.
 */
typedef struct route_t {
    struct stop_t *root_stop;
    struct trip_t *root_trip;
} Route;

/*
 * Network
 * A node is a stop location used by different routes.
 */
typedef struct network_t {
    struct node_t **nodes;
    size_t node_counter;
    size_t node_size;
    struct route_t **routes;
    size_t route_counter;
    size_t route_size;
} Network;

/*
 * Constructor-like helpers
 */

// Create a Network struct
Network *new_network();

// Create a Node struct and add it to the network
Node *new_node(Network *network, char *name, double x, double y);

// Create a Stop struct
Stop *new_stop(Node *node, Stop *last, Stop *next, int time_to_next);

// Create a Trip struct
Trip *new_trip(int departure, int capacity, Trip *next, Route *route);

// Create a Route struct and add it to the network
Route *new_route(Network *network, Node *nodes[], int times[],
                 size_t route_size, int departures[], int capacities[],
                 size_t trip_size);

/*
 * Print helpers
 */

// Print Node
void print_node(Node *node);

// Print Stop
void print_stop(Stop *stop);

// Print Trip
void print_trip(Trip *trip);

// Print Route
void print_route(Route *route);

// Print Network
void print_network(Network *network);

#endif  // NETWORK_H_
