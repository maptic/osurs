/**
 * @brief Seat reservation optimization.
 * @file optimize.c
 * @date: 2022-08-22
 * @author: Tobias Meier
 */

#include "osurs/optimize.h"

// Public definitions

// Seat constructor
Seat* new_seat(int seat_id) {
    Seat* seat = (Seat*)malloc(sizeof(Seat));
    seat->res_count = 0;
    seat->seat_id = seat_id;
    seat->res_id_arr = (int*)malloc(sizeof(unsigned int) * 8);
    return seat;
}

// Add a new reservation to a seat
void seat_add_reservation(Seat* seat, int res_id) {
    seat->res_id_arr[seat->res_count] = res_id;
    seat->res_count++;
}

// Seat destructor
void delete_seat(Seat* seat) {
    if (seat == NULL) return;
    free(seat->res_id_arr);
    free(seat);
}

// Seat collection constructor
SeatCollection* new_seat_collection(int seat_count, int seat_ids[]) {
    SeatCollection* collection =
        (SeatCollection*)malloc(sizeof(SeatCollection));
    collection->seat_count = seat_count;
    collection->seat_arr = (Seat**)malloc(seat_count * sizeof(Seat*));
    for (int i = 0; i < seat_count; ++i) {
        collection->seat_arr[i] = new_seat(seat_ids[i]);
    }
    return collection;
}

// Seat collection destructor
void delete_seat_collection(SeatCollection* collection) {
    for (int i = 0; i < collection->seat_count; ++i) {
        delete_seat(collection->seat_arr[i]);
    }
    free(collection->seat_arr);
    free(collection);
}

int space_available(unsigned int res_arr[], int res_count, int segment_count,
                    unsigned int seat_count, unsigned int new_res) {
    // iterate over each segment
    for (int i = 0; i < segment_count; ++i) {
        // sum all reservations for the specific segment
        unsigned int sum = 0;
        for (int j = 0; j < res_count; ++j) {
            sum += (res_arr[j] & ((unsigned int)1 << i)) >> i;
        }
        sum += (new_res & ((unsigned int)1 << i)) >> i;
        // if the sum exceeds the available space return 0
        if (sum > seat_count) {
            return 0;
        }
    }
    return 1;
}

SeatCollection* optimize_reservation(unsigned int res_arr[], int res_arr_count,
                                     int res_ids[], int segment_count,
                                     int seat_ids[], int seat_count) {
    // parameter check
    if (res_arr_count <= 0) {
        return NULL;
    }

    // datatype size check
    if (segment_count > sizeof(unsigned int) * 8) {
        return NULL;
    }

    SeatCollection* collection = new_seat_collection(seat_count, seat_ids);

    // iterate over each seat
    for (int i = 0; i < seat_count; ++i) {
        unsigned int current_res_config = 0;

        // iterate over each reservation
        for (int j = 0; j < res_arr_count; ++j) {
            //
            if (res_arr[j] != 0) {
                // break if a seat is fully booked (over all segments)
                // fully booked = logical representation of all ones
                // all ones = 2^segment_count - 1
                if (current_res_config == ((1 << segment_count) - 1)) {
                    break;
                }
                // add the reservation to the current seat if there is space
                // bitwise AND equals to 0 if there is no overlap
                if ((current_res_config & res_arr[j]) == 0) {
                    // add the logical representation of the current reservation
                    // to the current seat representation (bitwise OR)
                    current_res_config |= res_arr[j];
                    // add the reservation id to the seat
                    seat_add_reservation(collection->seat_arr[i], res_ids[j]);
                    // remove the reservation from the array
                    res_arr[j] = 0;
                }
            }
        }
    }
    return collection;
}