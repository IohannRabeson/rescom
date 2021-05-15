#define CATCH_CONFIG_MAIN

#include <catch2/catch_all.hpp>

#include "rescom.hpp"

TEST_CASE("EmptyProjectTests", "[simple]") {
    auto const& slot = rescom::getResource("test.txt");

    REQUIRE( slot.bytes == nullptr );
    REQUIRE( slot.size == 0 );
    REQUIRE( slot.key == nullptr );
}

