#define CATCH_CONFIG_MAIN

#include <catch2/catch_all.hpp>

#include "rescom.hpp"

TEST_CASE("getResource", "[TextOnly]") {
    auto const& slot = rescom::getResource("test.txt");

    REQUIRE( std::string(slot.bytes, slot.size) == "Hello world!" );
}

TEST_CASE("getText", "[TextOnly]") {
    REQUIRE( rescom::getText("test.txt").compare("Hello world!") == 0 );
}

TEST_CASE("invalid key", "[TextOnly]") {
    auto const& slot = rescom::getResource("test");

    REQUIRE( slot.bytes == nullptr );
    REQUIRE( slot.size == 0 );
    REQUIRE( slot.key == nullptr );
}

TEST_CASE("invalid key getText", "[TextOnly]") {
    REQUIRE( rescom::getText("test invalid.txt").empty() );
}

TEST_CASE("contains", "[TextOnly]") {
    REQUIRE( rescom::contains("test.txt") == true );
}

TEST_CASE("invalid key contains", "[TextOnly]") {
    REQUIRE( rescom::contains("test invalid.txt") == false );
}