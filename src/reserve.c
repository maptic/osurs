/**
 * @brief Connection routing, checking seat availability and reservation
 * @file reserve.c
 * @date: 2022-07-12
 * @author: Merlin Unterfinger
 */

#include <osurs/reserve.h>

// Private declarations

int min(int a, int b);

Stop *iterate_to_orig(Stop *curr_stop, const Node *orig, int *departure);

Stop *iterate_to_dest(Stop *curr_stop, const Node *dest, int *arrival,
                      int *available, int capacity, int trip_count);

Connection *search_trip(Connection *conn, const Node *orig, const Node *dest,
                        int time, Trip *trip, Stop *root_stop, int trip_count,
                        int *conn_count);

Connection *search_route(Connection *conn, const Node *orig, const Node *dest,
                         int time, Route *route, int cutoff);

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
    for (size_t i = 0; i < orig->route_counter; ++i) {
        for (size_t j = 0; j < dest->route_counter; ++j) {
            if (orig->routes[i] == dest->routes[j]) {
                conn = search_route(conn, orig, dest, time, orig->routes[i],
                                    INT_MAX);
            }
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
    int capacity = connection->trip->vehicle->composition->seats;
    while (1) {
        if (seats > (capacity - curr_stop->reserved[*trip_count])) return 0;
        // Stop if destination is reached
        if (curr_stop == connection->dest) break;
        curr_stop = curr_stop->next;
    }

    return 1;
}

int new_reservation(Connection *connection, int seats) {
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

void delete_connection(Connection *connection) {
    if (connection == NULL) return;

    Connection *prev_conn;
    Connection *next_conn;
    Connection *curr_conn;
    Connection *root_conn = connection;

    // Check for next
    curr_conn = root_conn->next;
    while (1) {
        if (curr_conn == NULL) break;
        next_conn = curr_conn->next;
        free(curr_conn);
        curr_conn = next_conn;
    }
    // Check for prev
    curr_conn = root_conn->prev;
    while (1) {
        if (curr_conn == NULL) break;
        prev_conn = curr_conn->prev;
        free(curr_conn);
        curr_conn = prev_conn;
    }
    // Finally free root
    free(root_conn);
}

// Private definitions

// Minimum of two integers
int min(int a, int b) { return (a > b) ? b : a; }

// Iterate to stop of origin node on trip
Stop *iterate_to_orig(Stop *curr_stop, const Node *orig, int *departure) {
    while (1) {
        if (curr_stop->node == orig) {
            *departure += curr_stop->departure_offset;
            return curr_stop;
        }
        curr_stop = curr_stop->next;
    }
}

// Iterate to stop of destination node on trip
Stop *iterate_to_dest(Stop *curr_stop, const Node *dest, int *arrival,
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

// Search for a connection on a trip
Connection *search_trip(Connection *conn, const Node *orig, const Node *dest,
                        int time, Trip *trip, Stop *root_stop, int trip_count,
                        int *conn_count) {
    Stop *orig_stop;
    Stop *dest_stop;
    int departure = trip->departure;
    int arrival;
    int available = INT_MAX;

    // Iterate over the stops until origin is reached
    orig_stop = iterate_to_orig(root_stop, orig, &departure);

    // Break if vehicle  missed
    if (departure < time) return conn;

    // Iterate over the stops until destination is reached.
    // Set departure at origin as base for calculation of the arrival at origin.
    arrival = departure;
    dest_stop = iterate_to_dest(orig_stop, dest, &arrival, &available,
                                trip->vehicle->composition->seats, trip_count);

    // Set in values of found connection
    *conn_count += 1;
    conn->trip = trip;
    conn->orig = orig_stop;
    conn->dest = dest_stop;
    conn->departure = departure;
    conn->arrival = arrival;
    conn->available = available;

    // Allocate next connection on heap; Set pointer to current connection as
    // prev connection of next connection.
    conn->next = (Connection *)malloc(sizeof(Connection));
    Connection *conn_prev = conn;
    conn = conn->next;
    conn->prev = conn_prev;

    return conn;
}

// Search for a connection on all trips of a route
Connection *search_route(Connection *conn, const Node *orig, const Node *dest,
                         int time, Route *route, int cutoff) {
    Trip *curr_trip = route->root_trip;
    int trip_count = 0;
    int conn_count = 0;

    while (1) {
        // Search trip if not already arrived at terminal
        if (curr_trip->arrival > time) {
            conn = search_trip(conn, orig, dest, time, curr_trip,
                               route->root_stop, trip_count, &conn_count);
        }
        if (conn_count >= cutoff || curr_trip->next == NULL) return conn;
        curr_trip = curr_trip->next;
        ++trip_count;
    }
    return conn;
}
