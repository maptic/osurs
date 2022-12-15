/**
 * @brief Optimize logic abstraction layer
 *
 * This module contains an abstraction layer between the optimization logic and
 * the planning/routing algorithm
 *
 * @file olal.h
 * @date: 2022-09-30
 * @author: Tobias Meier
 */

#ifndef OSURS_OLAL_H_
#define OSURS_OLAL_H_

#include "osurs/optimize.h"
#include "osurs/reserve.h"

/**
 * @brief Optimize seat reservations on given trip
 *
 * @param t The trip that needs to be optimized.
 *
 * @return A pointer to the optimized seat collection.
 **/
SeatCollection* optimize_trip(Trip* t);

#endif  // OSURS_OLAL_H_