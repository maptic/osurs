#include <gtest/gtest.h>
#include <limits.h>
#include <stdio.h>
#include <unistd.h>

extern "C" {
#include <osurs/io.h>
}

// Read a network
TEST(IOTest, Matsim) {
    // File paths
    int success;
    const char* schedule_file = "input/matsim/transitSchedule.xml";
    const char* vehicle_file = "input/matsim/transitVehicles.xml";

    // Init network
    Network* network = new_network();

    // Read files
    success = import_matsim(network, schedule_file, vehicle_file);

    // Test
    EXPECT_EQ(success, 1);
    EXPECT_EQ(network->node_counter, 17);
    EXPECT_EQ(network->route_counter, 2);

    // Cleanup
    delete_network(network);
}

TEST(IOTest, Read) {
    EXPECT_EQ(print_cwd(), 1);
    EXPECT_EQ(print_file("input/matsim/transitSchedule.xml"), 1);
    EXPECT_EQ(print_file("input/matsim/transitVehicles.xml"), 1);
}

// Wrtie a network
TEST(IOTest, Write) {
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}