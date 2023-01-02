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
    hash_map_free(node->routes);
    free(node);
}

void delete_network(Network *network) {
    // Free routes
    for (size_t i = 0; i < network->routes->capacity; i++) {
        HashMapEntry *entry = network->routes->entries[i];
        while (entry != NULL) {
            delete_route((Route *)entry->value);
            entry = entry->next;
        }
    }
    // Free vehicles
    for (size_t i = 0; i < network->vehicles->capacity; i++) {
        HashMapEntry *entry = network->vehicles->entries[i];
        while (entry != NULL) {
            delete_vehicle((Vehicle *)entry->value);
            entry = entry->next;
        }
    }
    // Free compositions
    for (size_t i = 0; i < network->compositions->capacity; i++) {
        HashMapEntry *entry = network->compositions->entries[i];
        while (entry != NULL) {
            delete_composition((Composition *)entry->value);
            entry = entry->next;
        }
    }
    // Free nodes
    for (size_t i = 0; i < network->nodes->capacity; i++) {
        HashMapEntry *entry = network->nodes->entries[i];
        while (entry != NULL) {
            delete_node((Node *)entry->value);
            entry = entry->next;
        }
    }
    // Free hashmaps
    hash_map_free(network->routes);
    hash_map_free(network->vehicles);
    hash_map_free(network->compositions);
    hash_map_free(network->nodes);
    // Free struct
    free(network);
}

// Private implementations

static void delete_stop(Stop *stop) {
    free(stop->reserved);
    free(stop);
}

static void delete_composition(Composition *composition) {
    if (composition != NULL) {
        free(composition->id);
        if (composition->seats != NULL)
        {
            for (int i = 0; i < composition->seat_count; ++i) {
                delete_seat(composition->seats[i]);
            }
            free(composition->seats);
            composition->seats = NULL;
        }
        free(composition);
        composition = NULL;
    }
}

static void delete_vehicle(Vehicle *vehicle) {
    free(vehicle->id);
    free(vehicle);
}

static void delete_trip(Trip *trip) {
    for (size_t i = 0; i < trip->reservations->size; ++i) {
        delete_reservation((Reservation*)array_list_get(trip->reservations, i));
    }
    array_list_free(trip->reservations);
    free(trip->id);
    free(trip);
}

static void delete_reservation(Reservation *reservation) {
    free(reservation->seat_arr);
    free(reservation); 
}

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

void delete_seat(Seat* seat) {
    if (seat == NULL) return;
    free(seat->res_arr);
    free(seat);
    seat = NULL;
}
