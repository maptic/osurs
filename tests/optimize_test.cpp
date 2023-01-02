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
    unsigned int res_count = 4;
    Reservation** res_arr = (Reservation**)malloc(sizeof(Reservation*) * res_count);
    for (int i = 0; i < res_count; ++i) {
        Reservation* res = (Reservation*)malloc(sizeof(Reservation));
        res->orig = NULL;
        res->dest = NULL;
        res->trip = NULL;
        res->seat_count = 1;
        res->seat_arr_index = 0;
        res->seat_arr = (Seat**)malloc(sizeof(Seat*) * res->seat_count);
        generate_uuid(res->id);
        res_arr[i] = res;
    }
    unsigned int log_res_arr[] = {3, 1, 6, 4};
    int max_segment_length = 3;
    int seat_count = 2;
    Seat* seats[2];
    for (int i = 0; i < seat_count; ++i) {
        seats[i] = new_seat(i + 100);
    }

    optimize_reservation(
        log_res_arr, res_count, res_arr, max_segment_length, seats, seat_count, fill);

    EXPECT_EQ(seats[0]->res_count, 2);
    EXPECT_EQ(seats[1]->res_count, 2);
    EXPECT_EQ(seats[0]->res_arr[0]->id, res_arr[0]->id);
    EXPECT_EQ(seats[0]->res_arr[1]->id, res_arr[3]->id);
    EXPECT_EQ(seats[1]->res_arr[0]->id, res_arr[1]->id);
    EXPECT_EQ(seats[1]->res_arr[1]->id, res_arr[2]->id);

    for (int i = 0; i < res_count; ++i) {
        free(res_arr[i]->seat_arr);
        free(res_arr[i]);
    }
    free(res_arr);
}