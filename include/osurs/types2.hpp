/**
 * @brief Data types of osurs.
 *
 * All data types used by osurs are declared and defined here.
 *
 * @file types.h
 * @date: 2022-07-12
 * @author: Merlin Unterfinger
 */

#ifndef OSURS_TYPES2_H_
#define OSURS_TYPES2_H_

#include <stdlib.h>

class SeatReservation
{

public:
	SeatReservation();
	SeatReservation(int sId);
	~SeatReservation();
	void AddReservation(int resId);
	int GetReservation(int pos);
	void SetSeatId(int sId);
	int GetSeatId() { return seatId; };
	int GetResCount() { return resCount; };
private:
	int seatId;
	int* resIdArr;
	int resCount;
};

class SeatReservationCollection
{
public:
	SeatReservationCollection(int count);
	SeatReservationCollection(int count, int ids[]);
	~SeatReservationCollection();
	SeatReservation* seatArr;
private:
	int getSeatCount() { return seatCount; };
	int seatCount = 0;
};

#endif  // OSURS_TYPES2_H_
