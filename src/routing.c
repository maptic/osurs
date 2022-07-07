/*
 * Network routing algorithm
 *
 * Author:  Merlin Unterfinger
 */

#include "routing.h"

#include <stdio.h>
#include <stdlib.h>

Connection *find_connection(Node *orig, Node *dest, int departure) {
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

                    // Iterate over the stops until origin of connection request
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
                                    // Stop if arrived at destination
                                    if (stop->node == dest) {
                                        // Fill in values for connection
                                        connection->trip = trip;
                                        connection->orig = orig_stop;
                                        connection->dest = stop;
                                        connection->departure = orig_departure;
                                        connection->arrival = arrival;
                                        connection->available = 9999;

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

    // Delete last empty connection to avoid memory leak.
    // Check if only one result
    if (connection->last != NULL) {
        connection->last->next = NULL;
        free(connection);
    }

    // Check for no results
    if (connection->last == NULL && connection->next == NULL) {
        return NULL;
    }

    // TODO: Sort connections according to arrival times.

    return root_connection;
}

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