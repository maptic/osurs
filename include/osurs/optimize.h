/**
 * @brief Optimization of space utilization on routs
 *
 * This module contains an algorithm to optimally distribute reservations along
 * a route to the available seats.
 *
 * @file optimize.h
 * @date: 2022-08-22
 * @author: Tobias Meier, Merlin Unterfinger
 */

#ifndef OSURS_OPTIMIZE_H_
#define OSURS_OPTIMIZE_H_

#include "osurs/types.h"
#include "osurs/network.h"

/** enum for the seat ordering method*/
typedef enum {
    fill,
    spatial_even
} order_method;

/**
 * @brief Adds a new reservation to the seat.
 *
 * @param seat The seat to which the reservation should be added.
 * @param res_id The reservation id.
 */
void seat_add_reservation(Seat* seat, Reservation* res);

/**
 * @brief Removes all the reservations of a seat.
 *
 * @param seat The seat from which all reservations should be removed.
 */
void seat_remove_reservations(Seat* seat);

/**
 * @brief Adds a new seat to the reservation.
 *
 * @param res The reservation to which the seat should be added.
 * @param seat The seat.
 */
void reservation_add_seat(Reservation* res, Seat* seat);

/**
 * @brief Removes all the seats of a reservation.
 *
 * @param res The reservation from which all seats should be removed.
 */
void reservations_remove_seats(Reservation* res);

/**
 * @brief Create a new seat collection.
 *
 * Initializes the seat collection properties and allocates the memory for the
 * seat array.
 *
 * @param seat_count The Number of seats.
 * @param seats[] Array that contains all the seat objects.
 * @return Returns a pointer to the new Seat_collection struct.
 */
SeatCollection* new_seat_collection(int seat_count, Seat** seats);

/**
 * @brief Delete a seat collection
 *
 * Frees the memory of the given seat collection on the heap.
 *
 * @param collection The seat collection to delete.
 */
void delete_seat_collection(SeatCollection* collection);

/**
 * @brief Check if there is enough space available
 *
 * Checks if the given reservation fits into the current configuration.
 *
 * @param log_res_arr[] The logical representation of each reservation.
 * @param res_count The number of reservations in the res_array.
 * @param segment_count The number of segments on the route. (stops - 1)
 * @param seat_count The number of seats.
 * @param new_res The logical representation of the new reservation.
 * @return Returns 1 if there is enough space available otherwise 0.
 */
int space_available(unsigned int log_res_arr[], int res_count, int segment_count,
                    unsigned int seat_count, unsigned int new_res);

/**
 * @brief Optimize the reservations
 *
 * Places the reservations on the seats in a way that optimizes the capacity.
 *
 * @param log_res_arr[] The logical representation of each reservation.
 * @param res_arr_count The number of reservations in the res_array.
 * @param res_ids[] Array that contains the reservation ids
 * @param segment_count The number of segments on the route. (stops - 1)
 * @param seat_ids[] Array that contains the seat ids
 * @param seat_count The number of seats in the composition.
 * @return Returns a pointer of the optimized Seat_collection.
 */
SeatCollection* optimize_reservation(unsigned int log_res_arr[], int res_arr_count,
                                     Reservation** res_ids, int segment_count,
                                     Seat** seats, int seat_count, order_method method);

#endif  // OSURS_OPTIMIZE_H_