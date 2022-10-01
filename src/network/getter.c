/**
 * @brief Access network structures.
 * @file getter.c
 * @date: 2022-09-30
 * @author: Merlin Unterfinger
 */

#include <osurs/network.h>
#include <stdio.h>
#include <string.h>

// Private declarations

// Public implementations

Node *get_node(Network *network, const char *id) {
    for (int i = 0; i < network->node_counter; ++i) {
        if (strcmp(network->nodes[i]->id, id) == 0) return network->nodes[i];
    }
    printf("Node %s not found.\n", id);
    return NULL;
}

Vehicle *get_vehicle(Network *network, const char *id) {
    for (int i = 0; i < network->vehicle_counter; ++i) {
        if (strcmp(network->vehicles[i]->id, id) == 0)
            return network->vehicles[i];
    }
    printf("Vehicle %s not found.\n", id);
    return NULL;
}

Composition *get_composition(Network *network, const char *id) {
    for (int i = 0; i < network->composition_counter; ++i) {
        if (strcmp(network->compositions[i]->id, id) == 0)
            return network->compositions[i];
    }
    printf("Composition %s not found.\n", id);
    return NULL;
}

Route *get_route(Network *network, const char *id) {
    for (int i = 0; i < network->route_counter; ++i) {
        if (strcmp(network->routes[i]->id, id) == 0) return network->routes[i];
    }
    printf("Route %s not found.\n", id);
    return NULL;
}

// Private implementations
