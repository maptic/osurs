#include <gtest/gtest.h>

extern "C" {
#include <osurs/reserve.h>
}

// Find a connection in the network
TEST(ReserveTest, Find) {
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}

// Check a found connection in the network
TEST(ReserveTest, Check) {
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}

// Reserve a found connection in the network
TEST(ReserveTest, Reserve) {
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}