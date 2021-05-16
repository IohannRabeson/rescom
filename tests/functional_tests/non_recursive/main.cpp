#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <catch2/catch_all.hpp>
#include "rescom.hpp"

std::string getString(std::string const& key)
{
    rescom::Resource const& slot = rescom::getResource(key.c_str());

    return std::string(slot.bytes, slot.size);
}

TEST_CASE("all", "NonRecursive") {
    REQUIRE( getString("test.txt") == "Hello world! root" );
    REQUIRE( getString("sub/test.txt") == "Hello sub world!" );
    REQUIRE( getString("sub sub/test.txt") == "Hello sub sub world!" );
    // This one is discovered because dir1 is listed
    REQUIRE( getString("dir1/test.txt") == "oh yeah" );
    // This one must be not found
    REQUIRE( rescom::getResource("dir1/recursive/test.txt").bytes == nullptr );
}

