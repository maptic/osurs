/**
 * @brief Routing connections between nodes on the network without transfers
 *
 * Public transit routing without transfers, checking seat availability and
 * selecting best connections based on arrival time.
 *
 * @file connection.c
 * @date: 2022-07-20
 * @author: Merlin Unterfinger
 */

#include <limits.h>

#include "osurs/reserve.h"

// Private declarations

static int min(int a, int b);

static Stop *iterate_to_orig(Stop *curr_stop, const Node *orig,
                             const Node *dest, int *departure);

static Stop *iterate_to_dest(Stop *curr_stop, const Node *dest, int *arrival,
                             int *available, int capacity, int trip_count);

static Connection *search_trip(Connection *conn, const Node *orig,
                               const Node *dest, int time, Trip *trip,
                               Stop *root_stop, int trip_count, int *conn_count,
                               int *direction);

static Connection *search_route(Connection *conn, const Node *orig,
                                const Node *dest, int time, Route *route,
                                int cutoff);

// Public definitions

Connection *new_connection(const Node *orig, const Node *dest, int time) {
    // Avoid same origin and destination
    if (orig == dest) {
        return NULL;
    }

    // Allocate connection root
    Connection *root_conn = (Connection *)malloc(sizeof(Connection));
    Connection *conn = root_conn;

    // Mark start of the chain
    conn->next = NULL;
    conn->prev = NULL;

    // Match and search on equal routes
    for (size_t i = 0; i < orig->routes->capacity; i++) {
        HashMapEntry *entry = orig->routes->entries[i];
        while (entry != NULL) {
            Route *route = (Route *)entry->value;
            if (hash_map_get(dest->routes, route->id) != NULL) {
                conn = search_route(conn, orig, dest, time, route, INT_MAX);
            }
            entry = entry->next;
        }
    }

    // Check for no results
    if (conn->prev == NULL && conn->next == NULL) {
        free(conn);
        return NULL;
    }

    // Avoid memory leak and delete last allocated empty connection
    conn->prev->next = NULL;
    free(conn);

    return root_conn;
}

int check_connection(Connection *connection, int seats, int *trip_count) {
    // Get trip number of route.
    Trip *curr_trip = connection->trip->route->root_trip;
    *trip_count = 0;
    while (1) {
        if (curr_trip == connection->trip) break;
        curr_trip = curr_trip->next;
        ++(*trip_count);
    }

    // Check available seats over on all visited stops.
    Stop *curr_stop = connection->orig;
    int capacity = connection->trip->vehicle->composition->seat_count;
    while (1) {
        // If less available seats than requested, return false.
        if (seats > (capacity - curr_stop->reserved[*trip_count])) return 0;
        // Stop if destination is reached.
        if (curr_stop == connection->dest) break;
        curr_stop = curr_stop->next;
    }

    return 1;
}

Connection *select_connection(Connection *connection, int seats) {
    Connection *prev_conn = connection;
    Connection *next_conn = connection;
    Connection *best_conn = NULL;
    int trip_count;

    // Set arrival time to maximum.
    int arrival = INT_MAX;

    // Check for next
    while (next_conn != NULL) {
        if ((next_conn->arrival < arrival) &&
            check_connection(next_conn, seats, &trip_count)) {
            arrival = next_conn->arrival;
            best_conn = next_conn;
        }
        next_conn = next_conn->next;
    }

    // Check for previous
    while (prev_conn != NULL) {
        if ((prev_conn->arrival < arrival) &&
            check_connection(prev_conn, seats, &trip_count)) {
            arrival = prev_conn->arrival;
            best_conn = prev_conn;
        }
        prev_conn = prev_conn->prev;
    }

    return best_conn;
}

void delete_connection(Connection *connection) {
    if (connection == NULL) return;

    Connection *prev_conn;
    Connection *next_conn;
    Connection *curr_conn;
    Connection *root_conn = connection;

    // Check for next
    curr_conn = root_conn->next;
    while (curr_conn != NULL) {
        next_conn = curr_conn->next;
        free(curr_conn);
        curr_conn = next_conn;
    }
    // Check for previous
    curr_conn = root_conn->prev;
    while (curr_conn != NULL) {
        prev_conn = curr_conn->prev;
        free(curr_conn);
        curr_conn = prev_conn;
    }
    // Finally free root
    free(root_conn);
}

// Private definitions

// Minimum of two integers.
static int min(int a, int b) { return (a > b) ? b : a; }

// Iterate to stop of origin node on trip.
static Stop *iterate_to_orig(Stop *curr_stop, const Node *orig,
                             const Node *dest, int *departure) {
    while (1) {
        if (curr_stop->node == orig) {
            *departure += curr_stop->departure_offset;
            return curr_stop;
        }
        if (curr_stop->node == dest) {
            return NULL;
        }
        curr_stop = curr_stop->next;
    }
}

// Iterate to stop of destination node on trip.
static Stop *iterate_to_dest(Stop *curr_stop, const Node *dest, int *arrival,
                             int *available, int capacity, int trip_count) {
    while (1) {
        *available =
            min(*available, capacity - curr_stop->reserved[trip_count]);
        if (curr_stop->node == dest) {
            *arrival += curr_stop->arrival_offset;
            return curr_stop;
        }
        curr_stop = curr_stop->next;
    }
}

// Search for a connection on a trip.
static Connection *search_trip(Connection *conn, const Node *orig,
                               const Node *dest, int time, Trip *trip,
                               Stop *root_stop, int trip_count, int *conn_count,
                               int *direction) {
    Stop *orig_stop;
    Stop *dest_stop;
    int departure = trip->departure;
    int arrival;
    int available = INT_MAX;

    // Iterate over the stops until origin is reached.
    orig_stop = iterate_to_orig(root_stop, orig, dest, &departure);

    // Break if vehicle missed.
    if (departure < time) return conn;

    // Break if wrong direction and set flag to skip route.
    if (orig_stop == NULL) {
        *direction = 1;
        return conn;
    }

    // Set departure at origin as base for calculation of the arrival at origin.
    arrival = departure;

    // Iterate over the stops until destination is reached.
    dest_stop =
        iterate_to_dest(orig_stop, dest, &arrival, &available,
                        trip->vehicle->composition->seat_count, trip_count);

    // Set values of found connection
    *conn_count += 1;
    conn->trip = trip;
    conn->orig = orig_stop;
    conn->dest = dest_stop;
    conn->departure = departure;
    conn->arrival = arrival;
    conn->available = available;

    // Allocate next connection on heap; Set pointer to current connection as
    // previous connection of next connection.
    conn->next = (Connection *)malloc(sizeof(Connection));
    Connection *conn_prev = conn;
    conn = conn->next;
    conn->prev = conn_prev;

    return conn;
}

// Search for a connection on all trips of a route.
static Connection *search_route(Connection *conn, const Node *orig,
                                const Node *dest, int time, Route *route,
                                int cutoff) {
    Trip *curr_trip = route->root_trip;
    int trip_count = 0;
    int conn_count = 0;
    int direction = 0;

    while (!direction) {
        // Search on trip if not already arrived at terminal.
        if (curr_trip->arrival > time) {
            conn =
                search_trip(conn, orig, dest, time, curr_trip, route->root_stop,
                            trip_count, &conn_count, &direction);
        }
        if (conn_count >= cutoff || curr_trip->next == NULL) return conn;
        curr_trip = curr_trip->next;
        ++trip_count;
    }
    return conn;
}
