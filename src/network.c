/*
 * Network for reservation optmization
 *
 * Author:  Merlin Unterfinger
 */

#include "osurs/network.h"

// Helpers to add references

void network_add_route(Network *network, Route *route) {
    if (network->route_counter == network->route_size) {
        printf("Need to reallocate size!\n");
        network->route_size += INIT_SIZE;
        network->routes = (Route **)realloc(
            network->routes, sizeof(Route *) * network->route_size);
    }
    network->routes[network->route_counter++] = route;
}

void network_add_node(Network *network, Node *node) {
    if (network->node_counter == network->node_size) {
        printf("Need to reallocate size!\n");
        network->node_size += INIT_SIZE;
        network->nodes = (Node **)realloc(network->nodes,
                                          sizeof(Node *) * network->node_size);
    }
    network->nodes[network->node_counter++] = node;
}

void node_add_route(Node *node, Route *route) {
    // Check if route already exists (only check last)
    if (node->routes[node->route_counter - 1] == route) return;
    if (node->route_counter == node->route_size) {
        printf("Need to reallocate size!\n");
        node->route_size += INIT_SIZE;
        node->routes =
            (Route **)realloc(node->routes, sizeof(Route *) * node->route_size);
    }
    node->routes[node->route_counter++] = route;
}

// Constructors

Network *new_network() {
    Network *network = (Network *)malloc(sizeof(Network));
    network->nodes = (Node **)malloc(sizeof(Node *) * INIT_SIZE);
    network->node_size = INIT_SIZE;
    network->node_counter = 0;
    network->routes = (Route **)malloc(sizeof(Route *) * INIT_SIZE);
    network->route_size = INIT_SIZE;
    network->route_counter = 0;
    return network;
}

Node *new_node(Network *network, char *name, double x, double y) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->name = name;
    node->x = x;
    node->y = y;
    node->routes = (Route **)malloc(sizeof(Route *) * INIT_SIZE);
    node->route_size = INIT_SIZE;
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

Trip *new_trip(int departure, int capacity, Trip *next, Route *route) {
    Trip *trip = (Trip *)malloc(sizeof(Trip));
    trip->departure = departure;
    trip->capacity = capacity;
    trip->next = next;
    trip->route = route;
    return trip;
}

Route *new_route(Network *network, Node *nodes[], int times[],
                 size_t route_size, int departures[], int capacities[],
                 size_t trip_size) {
    // Initialize route
    Route *route = (Route *)malloc(sizeof(Route));

    // Set root stop
    Stop *root_stop = new_stop(nodes[0], NULL, NULL, times[0], trip_size);
    route->root_stop = root_stop;

    // Set root trip
    Trip *root_trip = new_trip(departures[0], capacities[0], NULL, route);
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
        curr_trip = new_trip(departures[i], capacities[i], NULL, route);
        last_trip->next = curr_trip;
        last_trip = curr_trip;
    }

    // Add route to network
    network_add_route(network, route);

    // Add route to nodes
    for (size_t i = 0; i < route_size; ++i) node_add_route(nodes[i], route);

    return route;
}

// Print helpers

void print_node(Node *node) {
    printf("- Node <name=%s, x=%.3f, y=%.3f, routes=%ld>\n", node->name,
           node->x, node->y, node->route_counter);
}

void print_stop(Stop *stop) {
    printf("  - Stop <name=%s, last=%s, next=%s>]\n", stop->node->name,
           stop->last != NULL ? stop->last->node->name : "NA",
           stop->next != NULL ? stop->next->node->name : "NA");
}

void print_trip(Trip *trip) {
    printf("  - Trip <departure=%d, capacity=%d>\n", trip->departure,
           trip->capacity);
}

void print_route(Route *route) {
    printf("- Route <Stops>:\n");
    Stop *curr_stop = route->root_stop;
    while (curr_stop != NULL) {
        print_stop(curr_stop);
        curr_stop = curr_stop->next;
    }
    printf("- Route <Trips>:\n");
    Trip *curr_trip = route->root_trip;
    while (curr_trip != NULL) {
        print_trip(curr_trip);
        curr_trip = curr_trip->next;
    }
}

void print_network(Network *network) {
    printf("Network <Nodes>:\n");
    for (size_t i = 0; i < network->node_counter; ++i) {
        print_node(network->nodes[i]);
    }
    printf("Network <Routes>:\n");
    for (size_t i = 0; i < network->route_counter; ++i) {
        print_route(network->routes[i]);
    }
}