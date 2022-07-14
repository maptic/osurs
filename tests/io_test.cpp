#include <gtest/gtest.h>
#include <limits.h>
#include <stdio.h>
#include <unistd.h>

extern "C" {
#include <osurs/io.h>
}

// Read a network
TEST(IOTest, Read) {
    // EXPECT_EQ(print_cwd(), 0);
    // EXPECT_EQ(print_file("input/matsim/transitSchedule.xml"), 0);
    // EXPECT_EQ(print_file("input/matsim/vehicles.xml"), 0);
    Network* network = new_network();
    EXPECT_EQ(import_matsim(network, "input/matsim/transitSchedule.xml",
                            "input/matsim/vehicles.xml"),
              0);
    print_network(network);
    delete_network(network);
}

// Wrtie a network
TEST(IOTest, Write) {
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}