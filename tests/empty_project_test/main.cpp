#include "rescom.hpp"

#include <gtest/gtest.h>

TEST(EmptyProjectTests, simple) {
    auto const& slot = rescom::getResource("test.txt");

    EXPECT_EQ( slot.buffer, nullptr );
    EXPECT_EQ( slot.size, 0 );
    EXPECT_EQ( slot.key, nullptr );
}

