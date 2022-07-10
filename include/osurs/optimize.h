/*
 * Optimization of space utilization on trips
 *
 * This module contains an algorithm to optimally distribute reservations along
 * a trip to the available seats.
 *
 * Authors: Tobias Meier, Merlin Unterfinger
 */

#ifndef OSURS_OPTIMIZE_H_
#define OSURS_OPTIMIZE_H_

/*
 * Seats store the optimized reservations on the seats.
 */
typedef struct seats_t {
    int some;
    double thin;
} Seats;

#endif  // OSURS_OPTIMIZE_H_