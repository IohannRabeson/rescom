#define CATCH_CONFIG_MAIN

#include <catch2/catch_all.hpp>
#include <rescom.hpp>

TEST_CASE("ensure unique", "[DuplicateTests]") {
    REQUIRE( std::distance(rescom::begin(), rescom::end()) == 1 );
}