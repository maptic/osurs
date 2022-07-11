/*
 * Network routing algorithm and reservation
 *
 * Author:  Merlin Unterfinger
 */

#include <osurs/reserve.h>

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

// Private methods

int min(int a, int b);

Connection *find(const Node *orig, const Node *dest, int departure) {
    Connection *root_connection = (Connection *)malloc(sizeof(Connection));
    Connection *connection = root_connection;

    // Avoid same origin and destination
    if (orig == dest) {
        return NULL;
    }

    // Mark start of the chain
    connection->next = NULL;
    connection->last = NULL;

    // Check equal routes
    for (size_t i = 0; i < orig->route_counter; ++i) {
        for (size_t j = 0; j < dest->route_counter; ++j) {
            // We have a route match!
            if (orig->routes[i] == dest->routes[j]) {
                // Check for first trip, which arrives at origin before the
                // departure time
                Route *route = orig->routes[i];
                Trip *trip = route->root_trip;
                int trip_count = 0;

                // Iterate over trips
                while (1) {
                    // Get trip departure time and set as arrival time of
                    // starting stop
                    int arrival = trip->departure;
                    int capacity = trip->capacity;
                    int available = INT_MAX;

                    // Iterate over the stops until origin of connection
                    // request
                    Stop *stop = route->root_stop;
                    while (1) {
                        // Stop if arrived at origin, no need to check for
                        // NULL since route must exists
                        if (stop->node == orig) {
                            // Check if arrival of vehicle is before departure,
                            // if yes create a new connection
                            if (arrival >= departure) {
                                Stop *orig_stop = stop;
                                int orig_departure = arrival;

                                // Follow stops until destination
                                while (1) {
                                    // Update availabilities, the minimum is the
                                    // bottle neck
                                    available = min(
                                        available,
                                        capacity - stop->reserved[trip_count]);

                                    // Check if more than 0 available seats
                                    // if (available <= 0) break;

                                    // Stop if arrived at destination
                                    if (stop->node == dest) {
                                        // Fill in values for connection
                                        connection->trip = trip;
                                        connection->orig = orig_stop;
                                        connection->dest = stop;
                                        connection->departure = orig_departure;
                                        connection->arrival = arrival;
                                        connection->available = available;

                                        // Create next connection and set
                                        // current connection as last
                                        // connection.
                                        connection->next = (Connection *)malloc(
                                            sizeof(Connection));
                                        Connection *connection_last =
                                            connection;
                                        connection = connection->next;
                                        connection->last = connection_last;
                                        break;
                                    }
                                    // Increase arrival time and set next stop
                                    arrival += stop->time_to_next;
                                    stop = stop->next;
                                }
                                break;
                            }
                            // Already missed the train... break!
                            break;
                        }

                        // Increase arrival time and set next stop
                        arrival += stop->time_to_next;
                        stop = stop->next;
                    }

                    // Break if no more trips
                    if (trip->next == NULL) break;

                    // Set next trip
                    trip = trip->next;
                    ++trip_count;
                }
            }
        }
    }

    // Check for no results
    if (connection->last == NULL && connection->next == NULL) {
        free(connection);
        return NULL;
    }

    // Delete last empty connection to avoid memory leak.
    // Check if only one result
    if (connection->last != NULL) {
        connection->last->next = NULL;
        free(connection);
    }

    // TODO: Sort connections according to arrival times.

    return root_connection;
}

int check(Connection *connection, int seats, int *trip_count) {
    // Get trip number of route
    Trip *curr_trip = connection->trip->route->root_trip;
    *trip_count = 0;
    while (1) {
        if (curr_trip == connection->trip) break;
        curr_trip = curr_trip->next;
        ++(*trip_count);
    }

    // Check available seats over on all stops
    Stop *curr_stop = connection->orig;
    int capacity = connection->trip->capacity;
    while (1) {
        if (seats > (capacity - curr_stop->reserved[*trip_count])) return 0;
        // Stop if destination is reached
        if (curr_stop == connection->dest) break;
        curr_stop = curr_stop->next;
    }

    return 1;
}

int reserve(Connection *connection, int seats) {
    int trip_count;
    if (connection == NULL || seats > connection->available ||
        !check(connection, seats, &trip_count))
        return 0;

    // Book connection on individual stops
    Stop *curr_stop = connection->orig;
    while (1) {
        // Increase reservation counter
        curr_stop->reserved[trip_count] += seats;
        // Stop if destination is reached
        if (curr_stop == connection->dest) break;
        curr_stop = curr_stop->next;
    }

    return 1;
}

// Helpers

int min(int a, int b) { return (a > b) ? b : a; }

// Print helpers

void print_connection(Connection *connection) {
    Connection *curr_connection = connection;
    if (connection == NULL) {
        printf("No connection found.\n");
        return;
    }
    printf("Connections for %s --> %s:\n", connection->orig->node->name,
           connection->dest->node->name);
    while (curr_connection != NULL) {
        printf("- Connection<departure=%d, arrival=%d, available=%d>\n",
               curr_connection->departure, curr_connection->arrival,
               curr_connection->available);
        curr_connection = curr_connection->next;
    }
}