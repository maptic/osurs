#include <gtest/gtest.h>

extern "C" {
#include <osurs/optimize.h>
#include <osurs/reserve.h>
#include "../src/reserve/uuid.h"
}

TEST(OptimizeTest, SpaceAvailable) {
    unsigned int log_res_arr[] = {3, 1, 6};
    int res_count = 3;
    int segment_count = 3;
    int seat_count = 2;
    unsigned int new_res = 4;
    EXPECT_EQ(
        space_available(log_res_arr, res_count, segment_count, seat_count, new_res),
        1);

    new_res = 1;
    EXPECT_EQ(
        space_available(log_res_arr, res_count, segment_count, seat_count, new_res),
        0);
}

TEST(OptimizeTest, OptimizeReservation) {
    Reservation** res_arr = (Reservation**)malloc(sizeof(Reservation*) * 4);
    for (int i = 0; i < 4; ++i) {
        Reservation* res = (Reservation*)malloc(sizeof(Reservation));
        res->orig = NULL;
        res->dest = NULL;
        res->trip = NULL;
        res->seats = 1;
        generate_uuid(res->id);
        res_arr[i] = res;
    }
    int res_ids[] = {10, 20, 30, 40};
    unsigned int log_res_arr[] = {3, 1, 6, 4};
    unsigned int res_count = 4;
    int max_segment_length = 3;
    int seat_count = 2;
    Seat* seats[2];
    for (int i = 0; i < seat_count; ++i) {
        seats[i] = new_seat(i + 100);
    }

    SeatCollection* result_collection = optimize_reservation(
        log_res_arr, res_count, res_arr, max_segment_length, seats, seat_count, fill);

    EXPECT_EQ(result_collection->seat_arr[0]->seat_id, seats[0]->seat_id);
    EXPECT_EQ(result_collection->seat_arr[1]->seat_id, seats[1]->seat_id);
    EXPECT_EQ(result_collection->seat_arr[0]->res_count, 2);
    EXPECT_EQ(result_collection->seat_arr[1]->res_count, 2);
    EXPECT_EQ(result_collection->seat_arr[0]->res_arr[0], res_arr[0]);
    EXPECT_EQ(result_collection->seat_arr[0]->res_arr[1], res_arr[3]);
    EXPECT_EQ(result_collection->seat_arr[1]->res_arr[0], res_arr[1]);
    EXPECT_EQ(result_collection->seat_arr[1]->res_arr[1], res_arr[2]);

    delete_seat_collection(result_collection);
}