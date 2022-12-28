/**
 * @brief Network construction.
 * @file constructor.c
 * @date: 2022-07-12
 * @author: Merlin Unterfinger
 */

#include <string.h>

#include "osurs/network.h"

// Private declarations

static Stop *new_stop(Node *node, Stop *prev, Stop *next, int arrival_offset,
                      int departure_offset, size_t trip_size);
static Trip *new_trip(const char *id, int departure, int arrival,
                      Vehicle *vehicle, Trip *next, Route *route);

static void network_add_route(Network *network, Route *route);
static void network_add_node(Network *network, Node *node);
static void network_add_vehicle(Network *network, Vehicle *vehicle);
static void network_add_composition(Network *network, Composition *composition);
static void node_add_route(Node *node, Route *route);

// Public implementations

Network *new_network() {
    Network *network = (Network *)malloc(sizeof(Network));
    // Nodes
    network->nodes = hash_map_create();
    // Routes
    network->routes = hash_map_create();
    // Compositions
    network->compositions = hash_map_create();
    // Vehicles
    network->vehicles = hash_map_create();

    return network;
}

Node *new_node(Network *network, const char *id, double x, double y) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->id = strdup(id);
    node->x = x;
    node->y = y;
    node->routes = (Route **)malloc(sizeof(Route *) * INIT_ALLOC_SIZE_S);
    node->route_size = INIT_ALLOC_SIZE_S;
    node->route_counter = 0;

    // Add node to network
    network_add_node(network, node);

    return node;
}

Route *new_route(Network *network, const char *id, Node *nodes[],
                 int arrival_offsets[], int departure_offsets[],
                 size_t route_size, const char *trip_ids[], int departures[],
                 Vehicle *vehicles[], size_t trip_size) {
    // Initialize route
    Route *route = (Route *)malloc(sizeof(Route));
    route->id = strdup(id);
    route->route_size = route_size;
    route->trip_size = trip_size;

    // Set root stop
    Stop *root_stop = new_stop(nodes[0], NULL, NULL, arrival_offsets[0],
                               departure_offsets[0], trip_size);
    route->root_stop = root_stop;

    // Set root trip
    Trip *root_trip = new_trip(trip_ids[0], departures[0],
                               departures[0] + arrival_offsets[route_size - 1],
                               vehicles[0], NULL, route);
    route->root_trip = root_trip;

    // Create chain of all stops
    Stop *prev_stop = root_stop;
    Stop *curr_stop;
    for (size_t i = 1; i < route_size; ++i) {
        curr_stop = new_stop(nodes[i], prev_stop, NULL, arrival_offsets[i],
                             departure_offsets[i], trip_size);
        prev_stop->next = curr_stop;
        prev_stop = curr_stop;
    }

    // Create chain of all trips
    Trip *prev_trip = root_trip;
    Trip *curr_trip;
    for (size_t i = 1; i < trip_size; ++i) {
        curr_trip = new_trip(trip_ids[i], departures[i],
                             departures[i] + arrival_offsets[route_size - 1],
                             vehicles[i], NULL, route);
        prev_trip->next = curr_trip;
        prev_trip = curr_trip;
    }

    // Add route to network
    network_add_route(network, route);

    // Add route to nodes
    for (size_t i = 0; i < route_size; ++i) node_add_route(nodes[i], route);

    return route;
}

Vehicle *new_vehicle(Network *network, const char *id,
                     Composition *composition) {
    Vehicle *vehicle = (Vehicle *)malloc(sizeof(Vehicle));
    vehicle->id = strdup(id);
    vehicle->composition = composition;

    // Add vehicle to network
    network_add_vehicle(network, vehicle);

    return vehicle;
}

Composition *new_composition(Network *network, const char *id, int seat_count) {
    Composition *composition = (Composition *)malloc(sizeof(Composition));
    composition->id = strdup(id);
    composition->seat_count = seat_count;
    // Generate a dummy seat_id array
    // Will be replaced with real seat id with final composition implementation
    composition->seat_ids = (int *)malloc(sizeof(int) * seat_count);
    for (int i = 0; i < seat_count; ++i) {
        composition->seat_ids[i] = i + 100;
    }

    // Add composition to network
    network_add_composition(network, composition);

    return composition;
}

// Private implementations

static Stop *new_stop(Node *node, Stop *prev, Stop *next, int arrival_offset,
                      int departure_offset, size_t trip_size) {
    Stop *stop = (Stop *)malloc(sizeof(Stop));
    stop->node = node;
    stop->prev = prev;
    stop->next = next;
    stop->arrival_offset = arrival_offset;
    stop->departure_offset = departure_offset;
    stop->reserved = (int *)calloc(trip_size, sizeof(int));  // Initialized to 0
    return stop;
}

static Trip *new_trip(const char *id, int departure, int arrival,
                      Vehicle *vehicle, Trip *next, Route *route) {
    Trip *trip = (Trip *)malloc(sizeof(Trip));
    trip->id = strdup(id);
    trip->departure = departure;
    trip->arrival = arrival;
    trip->vehicle = vehicle;
    trip->next = next;
    trip->route = route;
    // Reservations
    trip->reservations = array_list_create();
    return trip;
}

static void network_add_route(Network *network, Route *route) {
    hash_map_put(network->routes, route->id, (void *)route);
}

static void network_add_node(Network *network, Node *node) {
    hash_map_put(network->nodes, node->id, (void *)node);
}

static void network_add_composition(Network *network,
                                    Composition *composition) {
    hash_map_put(network->compositions, composition->id, (void *)composition);
}

static void network_add_vehicle(Network *network, Vehicle *vehicle) {
    hash_map_put(network->vehicles, vehicle->id, (void *)vehicle);
}

static void node_add_route(Node *node, Route *route) {
    // Check if route already exists (only check last)
    if (node->route_counter > 0 &&
        node->routes[node->route_counter - 1] == route)
        return;
    if (node->route_counter == node->route_size) {
        node->route_size += INIT_ALLOC_SIZE_S;
        // printf("Reallocating route size of node \"%s\" (%ldx%ld bytes).\n",
        //        node->id, node->route_size, sizeof(void *));
        node->routes =
            (Route **)realloc(node->routes, sizeof(Route *) * node->route_size);
    }
    node->routes[node->route_counter++] = route;
}
