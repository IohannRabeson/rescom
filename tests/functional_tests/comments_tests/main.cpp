#define CATCH_CONFIG_MAIN

#include <catch2/catch_all.hpp>

#include "rescom.hpp"

TEST_CASE("count", "[Comments]") {
    REQUIRE( std::distance(rescom::begin(), rescom::end()) == 2 );
}

TEST_CASE("rescom file parsing", "[Comments]") {
    REQUIRE( std::string(rescom::getText("test.txt")) == "Hello world!" );
    REQUIRE( std::string(rescom::getText("test2.txt")) == "Yayayayay" );
}

