#include "rescom.hpp"

#include <gtest/gtest.h>

TEST(ProjectTests, simple) {
    auto const& slot = rescom::getResource("test.txt");

    EXPECT_EQ( std::string_view(slot.buffer, slot.size), "Hello world!" );
}

