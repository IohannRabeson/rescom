#include <Strings.hpp>
#include <catch2/catch_all.hpp>

TEST_CASE("trim", "StringTests") {
    REQUIRE( std::string(trim("")) == "" );
    REQUIRE( std::string(trim("  ")) == "" );
    REQUIRE( std::string(trim(" a b")) == "a b" );
    REQUIRE( std::string(trim("a b ")) == "a b" );
    REQUIRE( std::string(trim(" a b ")) == "a b" );
}

TEST_CASE("clean_comment", "StringTests")
{
    REQUIRE( std::string(removeComment("", "#")) == "" );
    REQUIRE( std::string(removeComment("#", "#")) == "" );
    REQUIRE( std::string(removeComment("a b#", "#")) == "a b" );
    REQUIRE( std::string(removeComment("a #b", "#")) == "a " );
    REQUIRE( std::string(removeComment("#a b ", "#")) == "" );
}
