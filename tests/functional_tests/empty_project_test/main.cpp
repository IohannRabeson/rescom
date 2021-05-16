#define CATCH_CONFIG_MAIN

#include <catch2/catch_all.hpp>

#include "rescom.hpp"

TEST_CASE("getResource", "[EmptyProjectTests]") {
    auto const& slot = rescom::getResource("test.txt");

    REQUIRE( slot.bytes == nullptr );
    REQUIRE( slot.size == 0 );
    REQUIRE( slot.key == nullptr );
}

TEST_CASE("getText", "[EmptyProjectTests]") {
    REQUIRE( rescom::getText("test.txt").empty() );
}

TEST_CASE("contains", "[TextOnly]") {
    REQUIRE( rescom::contains("") == false );
}

TEST_CASE("iterators", "[EmptyProjectTests]") {
    REQUIRE( std::distance(rescom::begin(), rescom::end()) == 0 );
}