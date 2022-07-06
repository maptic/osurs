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

#include "network.h"

Node *new_node(char *name, double x, double y) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->name = name;
    node->x = x;
    node->y = y;
    return node;
}

Stop *new_stop(Node *node, Stop *last, Stop *next, int time_to_next) {
    Stop *stop = (Stop *)malloc(sizeof(Stop));
    stop->node = node;
    stop->last = last;
    stop->next = next;
    stop->time_to_next = time_to_next;
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

Route *new_route(Node *nodes[], int times[], size_t route_size,
                 int departures[], int capacities[], size_t trip_size) {
    // Initialize route
    Route *route = (Route *)malloc(sizeof(Route));

    // Set root stop
    Stop *root_stop = new_stop(nodes[0], NULL, NULL, times[0]);
    route->root_stop = root_stop;

    // Set root trip
    Trip *root_trip = new_trip(departures[0], capacities[0], NULL, route);
    route->root_trip = root_trip;

    // Create chain of all stops
    Stop *last_stop = root_stop;
    Stop *curr_stop;
    for (int i = 1; i < route_size; ++i) {
        curr_stop = new_stop(nodes[i], last_stop, NULL, times[i]);
        last_stop->next = curr_stop;
        last_stop = curr_stop;
    }

    // Create chain of all trips
    Trip *last_trip = root_trip;
    Trip *curr_trip;
    for (int i = 1; i < trip_size; ++i) {
        curr_trip = new_trip(departures[i], capacities[i], NULL, route);
        last_trip->next = curr_trip;
        last_trip = curr_trip;
    }

    return route;
}

void print_node(Node *node) {
    printf("Node <name=%s, x=%.3f, y=%.3f>\n", node->name, node->x, node->y);
}

void print_stop(Stop *stop) {
    printf("Stop <name=%s, last=%s, next=%s>]\n", stop->node->name,
           stop->last != NULL ? stop->last->node->name : "NA",
           stop->next != NULL ? stop->next->node->name : "NA");
}

void print_trip(Trip *trip) {
    printf("Trip <departure=%d, capacity=%d>\n", trip->departure,
           trip->capacity);
}

void print_route(Route *route) {
    printf("Route <Stops>:\n");
    Stop *curr_stop = route->root_stop;
    while (curr_stop != NULL) {
        print_stop(curr_stop);
        curr_stop = curr_stop->next;
    }
    printf("Route <Trips>:\n");
    Trip *curr_trip = route->root_trip;
    while (curr_trip != NULL) {
        print_trip(curr_trip);
        curr_trip = curr_trip->next;
    }
}
