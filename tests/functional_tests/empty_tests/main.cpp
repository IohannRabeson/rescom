#define CATCH_CONFIG_MAIN

#include <catch2/catch_all.hpp>
#include <rescom.hpp>

TEST_CASE("getResource", "[EmptyTests]") {
    rescom::Resource const& slot = rescom::getResource("test.txt");

    REQUIRE( slot.bytes == nullptr );
    REQUIRE( slot.size == 0 );
    REQUIRE( slot.key == nullptr );
}

TEST_CASE("getText", "[EmptyTests]") {
    REQUIRE( rescom::getText("test.txt").empty() );
}

TEST_CASE("getResource null key", "[EmptyTests]") {
    rescom::Resource const& slot = rescom::getResource(nullptr);

    REQUIRE( slot.bytes == nullptr );
    REQUIRE( slot.size == 0 );
    REQUIRE( slot.key == nullptr );
}

TEST_CASE("iterators", "[EmptyTests]") {
    REQUIRE( std::distance(rescom::begin(), rescom::end()) == 0 );
}