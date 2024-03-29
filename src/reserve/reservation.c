/**
 * @brief Placing reservations for found connections.
 * @file reservation.c
 * @date: 2022-07-20
 * @author: Merlin Unterfinger
 */

#include <string.h>

#include "osurs/reserve.h"
#include "uuid.h"

// Private declarations

static void trip_add_reservation(Trip *trip, Reservation *reservation);
static int get_next_id();

// Public definitions

Reservation *new_reservation(Connection *connection, int seats, char *id) {
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

    // Generate UUID.
    if (id == NULL) {
        generate_uuid(res->id);
    } else {
        strcpy(res->id, id);
    }

    // TODO: move to olal.
    res->res_id = get_next_id();

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
    array_list_add(trip->reservations, (void *)reservation);
}

static int get_next_id() {
    static int count = 0;
    return count++;
};
