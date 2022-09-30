#include <gtest/gtest.h>

extern "C" {
#include <osurs/optimize.h>
}

TEST(OptimizeTest, SpaceAvailable) {
	int resCount = 3;
	unsigned int resArr[] = { 3, 1, 6 };
	int segmentCount = 3;
	int seatCount = 2;
	unsigned int newRes = 4;
	EXPECT_EQ(space_available(resArr, resCount, segmentCount, seatCount, newRes), 1);

	newRes = 1;
	EXPECT_EQ(space_available(resArr, resCount, segmentCount, seatCount, newRes), 0);
}

TEST(OptimizeTest, OptimizeReservation) {
	int resIds[] = { 10, 20, 30, 40 };
	unsigned int resArr2[] = { 3, 1, 6, 4 };
	unsigned int resCount = 4;
	int seatIds[] = { 100, 200 };
	int seatCount = 2;
	int maxSegmentLengeth = 3;

	Seat_collection* result_collection = optimize_reservation(resArr2, resCount, resIds, maxSegmentLengeth, seatIds, seatCount);

	EXPECT_EQ(result_collection->seat_arr[0]->seat_id, seatIds[0]);
	EXPECT_EQ(result_collection->seat_arr[1]->seat_id, seatIds[1]);
	EXPECT_EQ(result_collection->seat_arr[0]->res_count, 2);
	EXPECT_EQ(result_collection->seat_arr[1]->res_count, 2);
	EXPECT_EQ(result_collection->seat_arr[0]->res_id_arr[0], 10);
	EXPECT_EQ(result_collection->seat_arr[0]->res_id_arr[1], 40);
	EXPECT_EQ(result_collection->seat_arr[1]->res_id_arr[0], 20);
	EXPECT_EQ(result_collection->seat_arr[1]->res_id_arr[1], 30);

	delete_seat_collection(result_collection);
}