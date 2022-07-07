/*
 * Network routing algorithm
 *
 * Author:  Merlin Unterfinger
 */

#include "routing.h"

Connection *find_connection(Node *orig, Node *dest, int departure) {
    Connection *root_connection = (Connection *)malloc(sizeof(Connection));

    // Check equal routes
    for (size_t i = 0; i < orig->route_counter; ++i) {
        for (size_t j = 0; j < dest->route_counter; ++j) {
            if (orig->routes[i] == dest->routes[j]) {
                printf("%ld, %ld: We have a route match!!!\n", i, j);

                // Check for first trip, which arrives at origin before the
                // departure time
                Route *route = orig->routes[i];
                Trip *trip = route->root_trip;
                int trip_count = 0;
                while (1) {
                    // Get trip departure time and set as arrival time of
                    // starting stop
                    int arrival = trip->departure;

                    // Iterate over the stops
                    Stop *stop = route->root_stop;
                    while (1) {
                        arrival += stop->time_to_next;
                        // Stop if arrived at destination, no need to check for
                        // NULL since route must exists
                        stop = stop->next;
                        if (stop->node == dest) {
                            printf("Arrived at %d (departure: %d) on stop <%s>\n", arrival, departure, stop->node->name);
                            if (arrival <= departure) {
                                printf("Creating connection!\n");
                            }
                            break;
                        }
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

    return root_connection;
}