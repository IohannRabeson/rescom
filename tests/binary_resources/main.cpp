#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <catch2/catch_all.hpp>
#include "rescom.hpp"

TEST_CASE("ProjectTests", "[simple]") {
    rescom::Resource const& slot = rescom::getResource("test.txt");

    REQUIRE( slot.bytes != nullptr );
    REQUIRE( slot.size != 0 );
}

