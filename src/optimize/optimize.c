#include <osurs/optimize.h>

Seat* new_seat(int seat_id) {
	Seat* seat = (Seat*)malloc(sizeof(Seat));
	seat->res_count = 0;
	seat->seat_id = seat_id;
	seat->res_id_arr = (int*)malloc(sizeof(unsigned int) * 8);
	return seat;
}

void seat_add_reservation(Seat* seat, int res_id) {
	seat->res_id_arr[seat->res_count] = res_id;
	seat->res_count++;
}

void delete_seat(Seat* seat) {
	if (seat == NULL) return;
	for (int i = 0; i < sizeof(unsigned int) * 8; ++i) {
		free(seat->res_id_arr[i]);
	}
	free(seat);
}

Seat_collection* new_seat_collection(int seat_count, int seat_ids[]) {
	Seat_collection* collection = (Seat_collection*)malloc(sizeof(Seat_collection));
	collection->seat_count = seat_count;
	collection->seat_arr = (Seat**)malloc(seat_count * sizeof(Seat*));
	for (int i = 0; i < seat_count; ++i) {
		collection->seat_arr[i] = new_seat(seat_ids[i]);
	}
	return collection;
}

void delete_seat_collection(Seat_collection* collection) {
	for (int i = 0; i < collection->seat_count; ++i) {
		delete_seat(collection->seat_arr[i]);
	}
	free(collection->seat_arr);
	free(collection);
}

int space_available(unsigned int res_arr[], int res_count, int trip_count, unsigned int seat_count, unsigned int new_res)
{
	for (int i = 0; i < trip_count; ++i)
	{
		unsigned int sum = 0;
		for (int j = 0; j < res_count; ++j)
		{
			sum += (res_arr[j] & ((unsigned int)1 << i)) >> i;
		}
		sum += (new_res & ((unsigned int)1 << i)) >> i;
		if (sum > seat_count)
		{
			return 0;
		}
	}
	return 1;
}

Seat_collection* optimize_reservation(unsigned int res_arr[], int res_arr_count, int res_ids[], int max_trip_length, int seat_ids[], int seat_count)
{
	if (res_arr_count <= 0)
	{
		return NULL;
	}

	// datatype size check
	if (max_trip_length > sizeof(unsigned int) * 8)
	{
		return NULL;
	}

	Seat_collection* seatCollection = new_seat_collection(seat_count, seat_ids);

	for (int i = 0; i < seat_count; ++i)
	{
		unsigned int currentResConfig = 0;

		for (int j = 0; j < res_arr_count; ++j)
		{
			if (res_arr[j] != 0)
			{
				if (currentResConfig == ((1 << max_trip_length) - 1))
				{
					break;
				}

				if ((currentResConfig & res_arr[j]) == 0)
				{
					currentResConfig |= res_arr[j];
					seat_add_reservation(seatCollection->seat_arr[i], res_ids[j]);
					res_arr[j] = 0;
				}
			}
		}
	}
	return seatCollection;
}