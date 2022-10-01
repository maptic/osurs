/**
 * @brief Optimize logic abstraction layer
 *
 * This module contains an abstraction layer between the optimization logic and the planning/routing algorithm
 *
 * @file olal.h
 * @date: 2022-09-30
 * @author: Tobias Meier
 */

#pragma once

#include <osurs/types.h>
#include <osurs/optimize.h>

 /**
 * @brief Optimize seat reservations on given trip
 * 
 * @param t The trip that needs to be optimized.
 * 
 * @return A pointer to the optimized seat collection.
 **/
Seat_collection* optimize_trip(Trip* t);

