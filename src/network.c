/**
 * @brief Network for reservation optmization
 * @file network.c
 * @date: 2022-07-12
 * @author: Merlin Unterfinger
 */

#include <osurs/network.h>
#include <stdio.h>
#include <string.h>

// Private methods

Stop *new_stop(Node *node, Stop *last, Stop *next, int time_to_next,
               size_t trip_size);
Trip *new_trip(const char *id, int departure, int capacity, Trip *next,
               Route *route);

void delete_node(Node *node);
void delete_stop(Stop *stop);
void delete_trip(Trip *trip);
void delete_route(Route *route);

void network_add_route(Network *network, Route *route);
void network_add_node(Network *network, Node *node);
void node_add_route(Node *node, Route *route);

// Constructor-like methods

Network *new_network() {
    Network *network = (Network *)malloc(sizeof(Network));
    network->nodes = (Node **)malloc(sizeof(Node *) * INIT_ALLOC_SIZE);
    network->node_size = INIT_ALLOC_SIZE;
    network->node_counter = 0;
    network->routes = (Route **)malloc(sizeof(Route *) * INIT_ALLOC_SIZE);
    network->route_size = INIT_ALLOC_SIZE;
    network->route_counter = 0;
    return network;
}

Node *new_node(Network *network, const char *id, double x, double y) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->id = strdup(id);
    node->x = x;
    node->y = y;
    node->routes = (Route **)malloc(sizeof(Route *) * INIT_ALLOC_SIZE);
    node->route_size = INIT_ALLOC_SIZE;
    node->route_counter = 0;

    // Add node to network
    network_add_node(network, node);

    return node;
}

Stop *new_stop(Node *node, Stop *last, Stop *next, int time_to_next,
               size_t trip_size) {
    Stop *stop = (Stop *)malloc(sizeof(Stop));
    stop->node = node;
    stop->last = last;
    stop->next = next;
    stop->time_to_next = time_to_next;
    stop->reserved = (int *)calloc(trip_size, sizeof(int));  // Initialized to 0
    return stop;
}

Trip *new_trip(const char *id, int departure, int capacity, Trip *next,
               Route *route) {
    Trip *trip = (Trip *)malloc(sizeof(Trip));
    trip->id = strdup(id);
    trip->departure = departure;
    trip->capacity = capacity;
    trip->next = next;
    trip->route = route;
    return trip;
}

Route *new_route(Network *network, const char *id, Node *nodes[], int times[],
                 size_t route_size, const char *trip_ids[], int departures[],
                 int capacities[], size_t trip_size) {
    // Initialize route
    Route *route = (Route *)malloc(sizeof(Route));
    route->id = strdup(id);

    // Set root stop
    Stop *root_stop = new_stop(nodes[0], NULL, NULL, times[0], trip_size);
    route->root_stop = root_stop;

    // Set root trip
    Trip *root_trip =
        new_trip(trip_ids[0], departures[0], capacities[0], NULL, route);
    route->root_trip = root_trip;

    // Create chain of all stops
    Stop *last_stop = root_stop;
    Stop *curr_stop;
    for (size_t i = 1; i < route_size; ++i) {
        curr_stop = new_stop(nodes[i], last_stop, NULL, times[i], trip_size);
        last_stop->next = curr_stop;
        last_stop = curr_stop;
    }

    // Create chain of all trips
    Trip *last_trip = root_trip;
    Trip *curr_trip;
    for (size_t i = 1; i < trip_size; ++i) {
        curr_trip =
            new_trip(trip_ids[i], departures[i], capacities[i], NULL, route);
        last_trip->next = curr_trip;
        last_trip = curr_trip;
    }

    // Add route to network
    network_add_route(network, route);

    // Add route to nodes
    for (size_t i = 0; i < route_size; ++i) node_add_route(nodes[i], route);

    return route;
}

// Getters

Node *get_node(Network *network, const char *id) {
    for (int i = 0; i < network->node_counter; ++i) {
        if (strcmp(network->nodes[i]->id, id) == 0) return network->nodes[i];
    }
    printf("Node %s not found.\n", id);
    return NULL;
}

// Destructor-like methods

void delete_node(Node *node) {
    free(node->id);
    free(node->routes);
    free(node);
}

void delete_stop(Stop *stop) {
    free(stop->reserved);
    free(stop);
}

void delete_trip(Trip *trip) {
    free(trip->id);
    free(trip);
}

void delete_route(Route *route) {
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

void delete_network(Network *network) {
    // Free routes
    for (size_t i = 0; i < network->route_counter; ++i) {
        delete_route(network->routes[i]);
    }
    // Free nodes
    for (size_t i = 0; i < network->node_counter; ++i) {
        delete_node(network->nodes[i]);
    }
    // Free arrays
    free(network->routes);
    free(network->nodes);
    // Free struct
    free(network);
}

// Helpers to create relations in the network

void network_add_route(Network *network, Route *route) {
    if (network->route_counter == network->route_size) {
        network->route_size += INIT_ALLOC_SIZE;
        network->routes = (Route **)realloc(
            network->routes, sizeof(Route *) * network->route_size);
    }
    network->routes[network->route_counter++] = route;
}

void network_add_node(Network *network, Node *node) {
    if (network->node_counter == network->node_size) {
        network->node_size += INIT_ALLOC_SIZE;
        network->nodes = (Node **)realloc(network->nodes,
                                          sizeof(Node *) * network->node_size);
    }
    network->nodes[network->node_counter++] = node;
}

void node_add_route(Node *node, Route *route) {
    // Check if route already exists (only check last)
    if (node->route_counter > 0 &&
        node->routes[node->route_counter - 1] == route)
        return;
    if (node->route_counter == node->route_size) {
        node->route_size += INIT_ALLOC_SIZE;
        node->routes =
            (Route **)realloc(node->routes, sizeof(Route *) * node->route_size);
    }
    node->routes[node->route_counter++] = route;
}
