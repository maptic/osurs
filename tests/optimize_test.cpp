#include <gtest/gtest.h>

extern "C" {
#include <osurs/optimize.h>
}

TEST(OptimizeTest, SpaceAvailable) {
    unsigned int res_arr[] = {3, 1, 6};
    int res_count = 3;
    int segment_count = 3;
    int seat_count = 2;
    unsigned int new_res = 4;
    EXPECT_EQ(
        space_available(res_arr, res_count, segment_count, seat_count, new_res),
        1);

    new_res = 1;
    EXPECT_EQ(
        space_available(res_arr, res_count, segment_count, seat_count, new_res),
        0);
}

TEST(OptimizeTest, OptimizeReservation) {
    int res_ids[] = {10, 20, 30, 40};
    unsigned int res_arr2[] = {3, 1, 6, 4};
    unsigned int res_count = 4;
    int max_segment_length = 3;
    int seat_count = 2;
    Seat* seats[2];
    for (int i = 0; i < seat_count; ++i) {
        seats[i] = new_seat(i + 100);
    }

    SeatCollection* result_collection = optimize_reservation(
        res_arr2, res_count, res_ids, max_segment_length, seats, seat_count);

    EXPECT_EQ(result_collection->seat_arr[0]->seat_id, seats[0]->seat_id);
    EXPECT_EQ(result_collection->seat_arr[1]->seat_id, seats[1]->seat_id);
    EXPECT_EQ(result_collection->seat_arr[0]->res_count, 2);
    EXPECT_EQ(result_collection->seat_arr[1]->res_count, 2);
    EXPECT_EQ(result_collection->seat_arr[0]->res_id_arr[0], 10);
    EXPECT_EQ(result_collection->seat_arr[0]->res_id_arr[1], 40);
    EXPECT_EQ(result_collection->seat_arr[1]->res_id_arr[0], 20);
    EXPECT_EQ(result_collection->seat_arr[1]->res_id_arr[1], 30);

    delete_seat_collection(result_collection);
}