#include <Strings.hpp>
#include <catch2/catch_all.hpp>

TEST_CASE("trim", "StringTests") {
    REQUIRE( trim("") == "" );
    REQUIRE( trim("  ") == "" );
    REQUIRE( trim(" a b") == "a b" );
    REQUIRE( trim("a b ") == "a b" );
    REQUIRE( trim(" a b ") == "a b" );
}

TEST_CASE("clean_comment", "StringTests")
{
    REQUIRE( removeComment("", "#") == "" );
    REQUIRE( removeComment("#", "#") == "" );
    REQUIRE( removeComment("a b#", "#") == "a b" );
    REQUIRE( removeComment("a #b", "#") == "a " );
    REQUIRE( removeComment("#a b ", "#") == "" );


}
