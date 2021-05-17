#define CATCH_CONFIG_MAIN

#include <catch2/catch_all.hpp>
#include <rescom.hpp>

TEST_CASE("getResource", "[NonEmptyTests]") {
    rescom::Resource const& slot = rescom::getResource("test.txt");

    REQUIRE( slot.bytes != nullptr );
    REQUIRE( slot.size != 0 );
    REQUIRE( slot.key != nullptr );
}

TEST_CASE("getText", "[NonEmptyTests]") {
    REQUIRE( std::string(rescom::getText("test.txt")) == "Hello world!" );
}

TEST_CASE("getResource invalid key", "[NonEmptyTests]") {
    rescom::Resource const& slot = rescom::getResource("test_invalid_key.txt");

    REQUIRE( slot.bytes == nullptr );
    REQUIRE( slot.size == 0 );
    REQUIRE( slot.key == nullptr );
}

TEST_CASE("getResource null key", "[NonEmptyTests]") {
    rescom::Resource const& slot = rescom::getResource(nullptr);

    REQUIRE( slot.bytes == nullptr );
    REQUIRE( slot.size == 0 );
    REQUIRE( slot.key == nullptr );
}

TEST_CASE("iterators", "[NonEmptyTests]") {
    REQUIRE( std::distance(rescom::begin(), rescom::end()) == 4 );
}

TEST_CASE("sub directories", "[NonEmptyTests]") {
    REQUIRE( std::string(rescom::getText("sub/test.txt")) == "Hello sub world!" );
    REQUIRE( std::string(rescom::getText("sub/sub sub/test.txt")) == "Hello sub sub world!" );
}