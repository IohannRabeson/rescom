#define CATCH_CONFIG_MAIN

#include <catch2/catch_all.hpp>

#include "rescom.hpp"

TEST_CASE("ProjectTests", "[simple]") {
    auto const& slot = rescom::getResource("test.txt");

    REQUIRE( std::string(slot.bytes, slot.size) == "Hello world!" );
}

