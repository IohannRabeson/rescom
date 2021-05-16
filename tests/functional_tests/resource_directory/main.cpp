#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <catch2/catch_all.hpp>
#include "rescom.hpp"

std::string getString(std::string const& key)
{
    rescom::Resource const& slot = rescom::getResource(key.c_str());

    return std::string(slot.bytes, slot.size);
}

TEST_CASE("global", "global test")
{
    REQUIRE( std::distance(rescom::begin(), rescom::end()) == 5 );
    REQUIRE( std::is_sorted(rescom::begin(), rescom::end(), [](auto const& left, auto const right) { return std::strcmp(left.key, right.key) < 0; }) );
}

TEST_CASE("all", "ResourceDirectory") {
    REQUIRE( getString("test.txt") == "Hello world! root" );
    REQUIRE( getString("sub/test.txt") == "Hello sub world!" );
    REQUIRE( getString("sub sub/test.txt") == "Hello sub sub world!" );
    REQUIRE( getString("dir1/test.txt") == "oh yeah" );
    REQUIRE( getString("dir1/recursive/test.txt") == "wowowow" );
}

