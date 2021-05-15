#include <Strings.hpp>
#include <catch2/catch_all.hpp>

TEST_CASE("StringTests", "trim") {
    REQUIRE( ::trim("") == "" );
    REQUIRE( ::trim("  ") == "" );
    REQUIRE( ::trim(" a b") == "a b" );
    REQUIRE( ::trim("a b ") == "a b" );
    REQUIRE( ::trim(" a b ") == "a b" );
}

