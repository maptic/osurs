/**
 * @brief Optimize logic abstraction layer
 *
 * This module contains an abstraction layer between the optimization logic and the planning/routing algorithm
 *
 * @file olal.h
 * @date: 2022-09-30
 * @author: Tobias Meier, Merlin Unterfinger
 */

#pragma once

#include <osurs/types.h>
#include <osurs/optimize.h>

/**
*
*
**/
Seat_collection* optimize_trip(Trip t);

