/**
 * @brief Optimization logic abstraction layer.
 * @file olal.c
 * @date: 2022-09-30
 * @author: Tobias Meier
 */

#include "osurs/olal.h"

// optimizes the seat reservations on the given trip
SeatCollection* optimize_trip(Trip* t) {
    // get first stop of the route
    Stop* current_stop = t->route->root_stop;
    // create a temporary array to store the logical representation of the
    // reservations
    unsigned int* temp_logical_res_arr =
        (unsigned int*)malloc(sizeof(unsigned int) * t->reservations->size);
    for (int i = 0; i < t->reservations->size; ++i) {
        temp_logical_res_arr[i] = 0;
    }
    // iterate over each stop in the route
    for (int i = 0; i < t->route->route_size; ++i) {
        // iterate over each seat in the reservation
        for (int j = 0; j < t->reservations->size; ++j) {
            // check if it is the first stop of the route
            if (i > 0) {
                // if (the current stop equals to the res-origin) or (the last
                // stop was reserved and the current stop does not equal to the
                // res-dest) then set the bit to 1
                if ((((Reservation*)t->reservations->elements[j])->orig ==
                     current_stop) ||
                    ((temp_logical_res_arr[j] & (1u << (i - 1))) &&
                     (current_stop !=
                      ((Reservation*)t->reservations->elements[j])->dest))) {
                    temp_logical_res_arr[j] |= (1u << i);
                }
            } else {
                // if the fist stop matches the reservation origin then set the
                // bit to 1
                if (((Reservation*)t->reservations->elements[j])->orig ==
                    current_stop) {
                    temp_logical_res_arr[j] |= 1u;
                }
            }
        }
        current_stop = current_stop->next;
    }

    // count the total number of seat reservations
    int used_seat_count = 0;
    for (int i = 0; i < t->reservations->size; ++i) {
        used_seat_count += ((Reservation*)t->reservations->elements[i])->seats;
    }

    // reshape the logical representation array and create the res_id array
    int* res_ids = (int*)malloc(sizeof(int) * used_seat_count);
    unsigned int* logical_res_arr =
        (unsigned int*)malloc(sizeof(unsigned int) * used_seat_count);
    int seat_pos = 0;
    for (int i = 0; i < t->reservations->size; ++i) {
        for (int j = 0; j < ((Reservation*)t->reservations->elements[i])->seats;
             ++j) {
            logical_res_arr[seat_pos] = temp_logical_res_arr[i];
            res_ids[seat_pos++] =
                ((Reservation*)t->reservations->elements[i])->res_id;
        }
    }

    // call the optimization method
    SeatCollection* result = optimize_reservation(
        logical_res_arr, t->reservations->size, res_ids,
        (int)t->route->route_size - 1, t->vehicle->composition->seat_ids,
        t->vehicle->composition->seat_count);

    free(res_ids);
    free(temp_logical_res_arr);
    free(logical_res_arr);

    return result;
}