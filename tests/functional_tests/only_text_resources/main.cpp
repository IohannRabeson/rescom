#define CATCH_CONFIG_MAIN

#include <catch2/catch_all.hpp>

#include "rescom.hpp"

TEST_CASE("basic", "[TextOnly]") {
    auto const& slot = rescom::getResource("test.txt");

    REQUIRE( std::string(slot.bytes, slot.size) == "Hello world!" );
}

TEST_CASE("invalid key", "[TextOnly]") {
    auto const& slot = rescom::getResource("test");

    REQUIRE( slot.bytes == nullptr );
    REQUIRE( slot.size == 0 );
    REQUIRE( slot.key == nullptr );
}

