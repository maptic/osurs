#include <gtest/gtest.h>

extern "C" {
#include <osurs/io.h>
}

// Read a network
TEST(IOTest, Read) {
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}

// Wrtie a network
TEST(IOTest, Write) {
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}