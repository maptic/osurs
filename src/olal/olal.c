#include <osurs/olal.h>

// 
Seat_collection* optimize_trip(Trip* t)
{

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// TEMP - REMOVE
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// input:	segments from trip -> route (iterate)
	//			seatid from trip -> vehicle -> composition.get_seats()
	//			reservation id from trip -> Reservation
	//			segments in reservation from trip -> Reservation
	//
	// output:	seat_collection (other form?)
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// TEMP - REMOVE
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!



	Stop* currentStop = t->route->root_stop;
	for (int i = 0; i < t->route->route_size; ++i)
	{
		// res_id array

		// logical representation array



		currentStop = currentStop->next;
	}


	return optimize_reservation(
		, 
		t->reservation_counter, 
		, 
		(int)t->route->route_size, 
		t->vehicle->composition->seat_ids, 
		t->vehicle->composition->seat_count);
}