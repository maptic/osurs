/**
 * @brief Access network structures.
 * @file getter.c
 * @date: 2022-09-30
 * @author: Merlin Unterfinger
 */

#include <stdio.h>
#include <string.h>

#include "osurs/network.h"

// Private declarations

// Public implementations

Node *get_node(Network *network, const char *id) {
    Node *node = (Node *)hash_map_get(network->nodes, id);
    if (node == NULL) {
        printf("Node %s not found.\n", id);
    }
    return node;
}

Vehicle *get_vehicle(Network *network, const char *id) {
    Vehicle *vehicle = (Vehicle *)hash_map_get(network->vehicles, id);
    if (vehicle == NULL) {
        printf("Vehicle %s not found.\n", id);
    }
    return vehicle;
}

Composition *get_composition(Network *network, const char *id) {
    Composition *compo = (Composition *)hash_map_get(network->compositions, id);
    if (compo == NULL) {
        printf("Composition %s not found.\n", id);
    }
    return compo;
}

Route *get_route(Network *network, const char *id) {
    Route *route = (Route *)hash_map_get(network->routes, id);
    if (route == NULL) {
        printf("Route %s not found.\n", id);
    }
    return route;
}

Trip *get_trip(Route *route, const char *id) {
    Trip *curr_trip = route->root_trip;
    while (curr_trip) {
        if (strcmp(curr_trip->id, id) == 0) return curr_trip;
        curr_trip = curr_trip->next;
    }
    printf("Trip %s not found.\n", id);
    return NULL;
}

// Private implementations
