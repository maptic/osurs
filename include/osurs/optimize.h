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

 /**
 * @brief Create a new seat.
 *
 * Initializes the seat properties and allocates the memory for the reservation array.
 *
 * @param seat_id The id of the seat.
 * @return Returns a pointer to the new Seat struct.
 */
Seat* new_seat(int seat_id);

/**
* @brief Adds a new reservation to the seat.
*
* Initializes the seat properties and allocates the memory for the reservation array.
*
* @param seat The seat to which the reservation sould be added.
* @param res_id The reservation id.
*/
void seat_add_reservation(Seat* seat, int res_id);

/**
 * @brief Delete a seat
 *
 * Frees the memory of the given seat on the heap.
 *
 * @param seat The seat to delete.
 */
void delete_seat(Seat* seat);

/**
* @brief Create a new seat collection.
*
* Initializes the seat collection properties and allocates the memory for the seat array.
*
* @param seat_count The Number of seats.
* @param seat_ids[] Array that contains all the seat ids.
* @return Returns a pointer to the new Seat_collection struct.
*/
SeatCollection* new_seat_collection(int seat_count, int seat_ids[]);

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
* @param res_arr[] The logical representation of each reservation.
* @param res_count The number of reservations in the res_array.
* @param segment_count The number of segments on the route. (stops - 1)
* @param seat_count The number of seats.
* @param new_res The logical representation of the new reservation.
* @return Returns 1 if there is enough space available otherwise 0.
*/
int space_available(
	unsigned int res_arr[],
	int res_count,
	int segment_count,
	unsigned int seat_count,
	unsigned int new_res);

/**
* @brief Optimize the reservations
*
* Places the reservations on the seats in a way that optimizes the capacity.
*
* @param res_arr[] The logical representation of each reservation.
* @param res_arr_count The number of reservations in the res_array.
* @param res_ids[] Array that contains the reservation ids
* @param segment_count The number of segments on the route. (stops - 1)
* @param seat_ids[] Array that contains the seat ids
* @param seat_count The number of seats in the composition.
* @return Returns a pointer of the optimized Seat_collection.
*/
SeatCollection* optimize_reservation(
	unsigned int res_arr[],
	int res_arr_count,
	int res_ids[],
	int segment_count,
	int seat_ids[],
	int seat_count);

#endif  // OSURS_OPTIMIZE_H_