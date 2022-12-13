#include <gtest/gtest.h>

extern "C" {
#include <osurs/optimize.h>
}

TEST(OptimizeTest, SpaceAvailable) {
	int res_count = 3;
	unsigned int res_arr[] = { 3, 1, 6 };
	int segment_count = 3;
	int seat_count = 2;
	unsigned int new_res = 4;
	EXPECT_EQ(space_available(res_arr, res_count, segment_count, seat_count, new_res), 1);

	new_res = 1;
	EXPECT_EQ(space_available(res_arr, res_count, segment_count, seat_count, new_res), 0);
}

TEST(OptimizeTest, OptimizeReservation) {
	int res_ids[] = { 10, 20, 30, 40 };
	unsigned int res_arr2[] = { 3, 1, 6, 4 };
	unsigned int res_count = 4;
	int seat_ids[] = { 100, 200 };
	int seat_count = 2;
	int max_segment_length = 3;

	SeatCollection* result_collection = optimize_reservation(res_arr2, res_count, res_ids, max_segment_length, seat_ids, seat_count);

	EXPECT_EQ(result_collection->seat_arr[0]->seat_id, seat_ids[0]);
	EXPECT_EQ(result_collection->seat_arr[1]->seat_id, seat_ids[1]);
	EXPECT_EQ(result_collection->seat_arr[0]->res_count, 2);
	EXPECT_EQ(result_collection->seat_arr[1]->res_count, 2);
	EXPECT_EQ(result_collection->seat_arr[0]->res_id_arr[0], 10);
	EXPECT_EQ(result_collection->seat_arr[0]->res_id_arr[1], 40);
	EXPECT_EQ(result_collection->seat_arr[1]->res_id_arr[0], 20);
	EXPECT_EQ(result_collection->seat_arr[1]->res_id_arr[1], 30);

	delete_seat_collection(result_collection);
}