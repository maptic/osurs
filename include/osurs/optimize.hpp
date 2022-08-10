/**
 * @brief Optimization of space utilization on trips
 *
 * This module contains an algorithm to optimally distribute reservations along
 * a trip to the available seats.
 *
 * @file optimize.h
 * @date: 2022-07-12
 * @author: Tobias Meier, Merlin Unterfinger
 */

#ifndef OSURS_OPTIMIZE_H_
#define OSURS_OPTIMIZE_H_

#include "osurs/types2.hpp"

int spaceAvailable(
	unsigned int resArr[],
	int resCount,
	int tripCount,
	unsigned int seatCount,
	unsigned int newRes);

SeatReservationCollection& optimizeReservation(
	unsigned int resArr[],
	int resArrCount,
	int resIds[],
	int maxTripLengeth,
	int seatIds[],
	int seatCount);

#endif  // OSURS_OPTIMIZE_H_