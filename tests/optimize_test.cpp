#include <gtest/gtest.h>
#include <osurs/optimize.hpp>


TEST(OptimizeTest, SpaceAvailable) {
	int resCount = 3;
	unsigned int resArr[] = { 3, 1, 6 };
	int tripCount = 3;
	int seatCount = 2;
	unsigned int newRes = 4;
	EXPECT_EQ(spaceAvailable(resArr, resCount, tripCount, seatCount, newRes), 1);

	newRes = 1;
	EXPECT_EQ(spaceAvailable(resArr, resCount, tripCount, seatCount, newRes), 0);
}

TEST(OptimizeTest, OptimizeReservation) {
	int resIds[] = { 10, 20, 30, 40 };
	unsigned int resArr2[] = { 3, 1, 6, 4 };
	unsigned int resCount = 4;
	int seatIds[] = { 100, 200 };
	int seatCount = 2;
	int maxTripLengeth = 3;

	SeatReservationCollection resultCollection = optimizeReservation(resArr2, resCount, resIds, maxTripLengeth, seatIds, seatCount);

	EXPECT_EQ(resultCollection.seatArr[0].GetSeatId(), seatIds[0]);
	EXPECT_EQ(resultCollection.seatArr[1].GetSeatId(), seatIds[1]);
	EXPECT_EQ(resultCollection.seatArr[0].GetResCount(), 2);
	EXPECT_EQ(resultCollection.seatArr[1].GetResCount(), 2);
	EXPECT_EQ(resultCollection.seatArr[0].GetReservation(0), 10);
	EXPECT_EQ(resultCollection.seatArr[0].GetReservation(1), 40);
	EXPECT_EQ(resultCollection.seatArr[1].GetReservation(0), 20);
	EXPECT_EQ(resultCollection.seatArr[1].GetReservation(1), 30);
}