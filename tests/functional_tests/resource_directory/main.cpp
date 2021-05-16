#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <catch2/catch_all.hpp>
#include "rescom.hpp"

std::string getString(std::string const& key)
{
    rescom::Resource const& slot = rescom::getResource(key.c_str());

    return std::string(slot.bytes, slot.size);
}

TEST_CASE("all", "ResourceDirectory") {
    REQUIRE( getString("test.txt") == "Hello world! root" );
    REQUIRE( getString("sub/test.txt") == "Hello sub world!" );
    REQUIRE( getString("sub sub/test.txt") == "Hello sub sub world!" );
    REQUIRE( getString("recursive/test.txt") == "recursive! oh yeah" );
}

