#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <catch2/catch_all.hpp>

#include "rescom.hpp"

TEST_CASE("ProjectTests", "[simple]") {
    auto const& slot = rescom::getResource("test.txt");

    REQUIRE( std::string(slot.bytes, slot.size) == "Hello world!" );
}

