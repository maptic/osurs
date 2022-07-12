#include <gtest/gtest.h>

// Avoid undefined-symbol errors at linktime
extern "C" {
#include <osurs/optimize.h>
}

// Demonstrate some basic assertions.
TEST(OptimizeTest, BasicAssertions) {
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}