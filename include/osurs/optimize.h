#pragma once

#include <osurs/types.h>

Seat* new_seat(int seat_id);

void seat_add_reservation(Seat* seat, int res_id);

void delete_seat(Seat* seat);

Seat_collection* new_seat_collection(int seat_count, int seat_ids[]);

void delete_seat_collection(Seat_collection* collection);

int space_available(
	unsigned int res_arr[],
	int res_count,
	int trip_count,
	unsigned int seat_count,
	unsigned int new_res);

Seat_collection* optimize_reservation(
	unsigned int res_arr[],
	int res_arr_count,
	int res_ids[],
	int max_trip_length,
	int seat_ids[],
	int seat_count);
