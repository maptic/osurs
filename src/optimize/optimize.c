/**
 * @brief Seat reservation optimization.
 * @file optimize.c
 * @date: 2022-08-22
 * @author: Tobias Meier
 */

#include "osurs/optimize.h"

// Public definitions

// Add a new reservation to a seat
void seat_add_reservation(Seat* seat, Reservation* res) {
    seat->res_arr[seat->res_count] = res;
    seat->res_count++;
}

// Remove all reservations of a seat
void seat_remove_reservations(Seat* seat) {
    for (int i = 0; i < seat->res_count; ++i) {
        seat->res_arr[i] = NULL;
    }
    seat->res_count = 0;
}

// Seat collection constructor
SeatCollection* new_seat_collection(int seat_count, Seat** seats) {
    SeatCollection* collection =
        (SeatCollection*)malloc(sizeof(SeatCollection));
    collection->seat_count = seat_count;
    collection->seat_arr = seats;
    return collection;
}

// Seat collection destructor
void delete_seat_collection(SeatCollection* collection) {
    free(collection);
    collection = NULL;
}

int space_available(unsigned int log_res_arr[], int res_count, int segment_count,
                    unsigned int seat_count, unsigned int new_res) {
    // iterate over each segment
    for (int i = 0; i < segment_count; ++i) {
        // sum all reservations for the specific segment
        unsigned int sum = 0;
        for (int j = 0; j < res_count; ++j) {
            sum += (log_res_arr[j] & ((unsigned int)1 << i)) >> i;
        }
        sum += (new_res & ((unsigned int)1 << i)) >> i;
        // if the sum exceeds the available space return 0
        if (sum > seat_count) {
            return 0;
        }
    }
    return 1;
}

SeatCollection* optimize_reservation(unsigned int log_res_arr[], int res_arr_count,
                                     Reservation** res_arr, int segment_count,
                                     Seat** seats, int seat_count, order_method method) {
    // parameter check
    if (res_arr_count <= 0) {
        return NULL;
    }

    // datatype size check
    if (segment_count > sizeof(unsigned int) * 8) {
        return NULL;
    }

    // unordered seat array
    Seat** unordered_seats;
    if (method != fill) {
        unordered_seats = (Seat*)malloc(sizeof(Seat) * seat_count);
        for (int i = 0; i < seat_count; ++i) {
            unordered_seats[i] = new_seat(0);
        }
    }

    // iterate over each seat
    for (int i = 0; i < seat_count; ++i) {
        unsigned int current_res_config = 0;

        // iterate over each reservation
        for (int j = 0; j < res_arr_count; ++j) {
            //
            if (log_res_arr[j] != 0) {
                // break if a seat is fully booked (over all segments)
                // fully booked = logical representation of all ones
                // all ones = 2^segment_count - 1
                if (current_res_config == ((1 << segment_count) - 1)) {
                    break;
                }
                // add the reservation to the current seat if there is space
                // bitwise AND equals to 0 if there is no overlap
                if ((current_res_config & log_res_arr[j]) == 0) {
                    // add the logical representation of the current reservation
                    // to the current seat representation (bitwise OR)
                    current_res_config |= log_res_arr[j];
                    // add the reservation id to the seat
                    if (method == fill) {
                        seat_add_reservation(seats[i], res_arr[j]);
                    } 
                    else {
                        seat_add_reservation(unordered_seats[i], res_arr[j]);
                    }
                    // remove the reservation from the array
                    log_res_arr[j] = 0;
                }
            }
        }
    }

    switch (method) {
    case fill:
        break;
    case spatial_even:
        // clear all persisting reservations on the given seats of the composition
        for (int i = 0; i < seat_count; ++i) {
            seat_remove_reservations(seats[i]);
        }

        // distribute the reserved seats spatial even over the seats given in the composition
        // TODO:

        // if it was an even seat count add the last seat to the last position of the array
        if (seat_count % 2 == 0) {
            //seats[seat_count -1]
        }

        for (int i = 0; i < seat_count; ++i) {
            free(unordered_seats[i]);
        }
        free(unordered_seats);

    }

    SeatCollection* collection = new_seat_collection(seat_count, seats);

    return collection;
}

