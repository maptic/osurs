/*
 * Network routing algorithm and reservation
 *
 * Author:  Merlin Unterfinger
 */

#include <osurs/reserve.h>

// Private methods

int min(int a, int b);

// Iterate to stop of origin node on trip.
Stop *iterate_to_orig(Stop *curr_stop, const Node *orig, int *departure) {
    while (1) {
        if (curr_stop->node == orig) return curr_stop;
        *departure += curr_stop->time_to_next;
        curr_stop = curr_stop->next;
    }
}

// Iterate to stop of destination node on trip.
Stop *iterate_to_dest(Stop *curr_stop, const Node *dest, int *arrival,
                      int *available, int capacity, int trip_count) {
    while (1) {
        *available =
            min(*available, capacity - curr_stop->reserved[trip_count]);
        if (curr_stop->node == dest) return curr_stop;
        *arrival += curr_stop->time_to_next;
        curr_stop = curr_stop->next;
    }
}

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
                                trip->capacity, trip_count);

    // Set in values of found connection
    *conn_count += 1;
    conn->trip = trip;
    conn->orig = orig_stop;
    conn->dest = dest_stop;
    conn->departure = departure;
    conn->arrival = arrival;
    conn->available = available;

    // Allocate next connection on heap; Set pointer to current connection as
    // last connection of next connection.
    conn->next = (Connection *)malloc(sizeof(Connection));
    Connection *conn_last = conn;
    conn = conn->next;
    conn->last = conn_last;

    return conn;
}

Connection *search_route(Connection *conn, const Node *orig, const Node *dest,
                         int time, Route *route, int cutoff) {
    Trip *curr_trip = route->root_trip;
    int trip_count = 0;
    int conn_count = 0;

    // TODO: Use a cutoff parameter for trip results per route on conn_count?
    while (1) {
        conn = search_trip(conn, orig, dest, time, curr_trip, route->root_stop,
                           trip_count, &conn_count);
        if (conn_count >= cutoff || curr_trip->next == NULL) return conn;
        curr_trip = curr_trip->next;
        ++trip_count;
    }
    return conn;
}

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
    conn->last = NULL;

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
    if (conn->last == NULL && conn->next == NULL) {
        free(conn);
        return NULL;
    }

    // Avoid memory leak and delete last allocated empty connection
    conn->last->next = NULL;
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

// Destructor-like methods

void delete_connection(Connection *connection) {
    if (connection == NULL) return;

    Connection *last_conn;
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
    // Check for last
    curr_conn = root_conn->last;
    while (1) {
        if (curr_conn == NULL) break;
        last_conn = curr_conn->last;
        free(curr_conn);
        curr_conn = last_conn;
    }
    // Finally free root
    free(root_conn);
}

void delete_node(Node *node);

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