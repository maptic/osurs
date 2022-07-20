/**
 * @brief Placing reservations for found connections.
 * @file reservation.c
 * @date: 2022-07-20
 * @author: Merlin Unterfinger
 */

#include <osurs/reserve.h>

// Private declarations

static void trip_add_reservation(Trip *trip, Reservation *reservation);

// Public definitions

Reservation *new_reservation(Connection *connection, int seats) {
    Reservation *res = NULL;
    int trip_count;

    // Check connection and seat availability.
    if (connection == NULL || seats > connection->available ||
        !check_connection(connection, seats, &trip_count))
        return res;

    // Allocate reservation struct.
    res = (Reservation *)malloc(sizeof(Reservation));
    res->orig = connection->orig;
    res->dest = connection->dest;
    res->trip = connection->trip;
    res->seats = seats;

    // Connect trip to reservation.
    trip_add_reservation(connection->trip, res);

    // Book connection on individual stops.
    Stop *curr_stop = connection->orig;
    while (1) {
        // Increase reservation counter.
        curr_stop->reserved[trip_count] += seats;
        // Stop if destination is reached.
        if (curr_stop == connection->dest) break;
        curr_stop = curr_stop->next;
    }

    return res;
}

// Private definitions

static void trip_add_reservation(Trip *trip, Reservation *reservation) {
    if (trip->reservation_counter == trip->reservation_size) {
        trip->reservation_size += INIT_ALLOC_SIZE_S;
        // printf("Reallocating reservation size of network (%ldx%ld bytes).\n",
        //        network->reservation_size, sizeof(void *));
        trip->reservations = (Reservation **)realloc(
            trip->reservations, sizeof(Reservation *) * trip->reservation_size);
    }
    trip->reservations[trip->reservation_counter++] = reservation;
}