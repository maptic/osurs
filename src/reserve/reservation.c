/**
 * @brief Placing reservations for found connections.
 * @file reservtion.c
 * @date: 2022-07-20
 * @author: Merlin Unterfinger
 */

#include <osurs/reserve.h>

// Private declarations

// Public definitions

int new_reservation(Connection *connection, int seats) {
    int trip_count;
    if (connection == NULL || seats > connection->available ||
        !check_connection(connection, seats, &trip_count))
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

// Private definitions
