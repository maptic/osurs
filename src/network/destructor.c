/**
 * @brief Network destruction.
 * @file destructor.c
 * @date: 2022-07-12
 * @author: Merlin Unterfinger
 */

#include "osurs/network.h"

// Private declarations

static void delete_node(Node *node);
static void delete_stop(Stop *stop);
static void delete_composition(Composition *composition);
static void delete_vehicle(Vehicle *vehicle);
static void delete_trip(Trip *trip);
static void delete_route(Route *route);
static void delete_reservation(Reservation *reservation);

// Public implementations

void delete_node(Node *node) {
    free(node->id);
    free(node->routes);
    free(node);
}

void delete_network(Network *network) {
    // Free routes
    for (size_t i = 0; i < network->route_counter; ++i) {
        delete_route(network->routes[i]);
    }
    // Free vehicles
    for (size_t i = 0; i < network->vehicle_counter; ++i) {
        delete_vehicle(network->vehicles[i]);
    }
    // Free compositions
    for (size_t i = 0; i < network->composition_counter; ++i) {
        delete_composition(network->compositions[i]);
    }
    // Free nodes
    for (size_t i = 0; i < network->node_counter; ++i) {
        delete_node(network->nodes[i]);
    }
    // Free arrays
    free(network->routes);
    free(network->vehicles);
    free(network->compositions);
    free(network->nodes);
    // Free struct
    free(network);
}

// Private implementations

static void delete_stop(Stop *stop) {
    free(stop->reserved);
    free(stop);
}

static void delete_composition(Composition *composition) {
    free(composition->id);
    free(composition->seat_ids);
    free(composition);
}

static void delete_vehicle(Vehicle *vehicle) {
    free(vehicle->id);
    free(vehicle);
}

static void delete_trip(Trip *trip) {
    for (size_t i = 0; i < trip->reservation_counter; ++i) {
        delete_reservation(trip->reservations[i]);
    }
    free(trip->reservations);
    free(trip->id);
    free(trip);
}

static void delete_reservation(Reservation *reservation) { free(reservation); }

static void delete_route(Route *route) {
    // Free stops
    Stop *next_stop;
    Stop *curr_stop = route->root_stop;
    while (1) {
        next_stop = curr_stop->next;
        delete_stop(curr_stop);
        if (next_stop == NULL) {
            break;
        }
        curr_stop = next_stop;
    }
    // Free trips
    Trip *next_trip;
    Trip *curr_trip = route->root_trip;
    while (1) {
        next_trip = curr_trip->next;
        delete_trip(curr_trip);
        if (next_trip == NULL) {
            break;
        }
        curr_trip = next_trip;
    }
    // Free struct
    free(route->id);
    free(route);
}
