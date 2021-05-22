#include <StringHelpers.hpp>
#include <catch2/catch_all.hpp>

TEST_CASE("trim", "StringTests") {
    REQUIRE( std::string(trim("")) == "" );
    REQUIRE( std::string(trim("  ")) == "" );
    REQUIRE( std::string(trim(" a b")) == "a b" );
    REQUIRE( std::string(trim("a b ")) == "a b" );
    REQUIRE( std::string(trim(" a b ")) == "a b" );
}

TEST_CASE("removeComment", "StringTests")
{
    REQUIRE( std::string(removeComment("", "#")) == "" );
    REQUIRE( std::string(removeComment("#", "#")) == "" );
    REQUIRE( std::string(removeComment("a b#", "#")) == "a b" );
    REQUIRE( std::string(removeComment("a #b", "#")) == "a " );
    REQUIRE( std::string(removeComment("#a b ", "#")) == "" );
}

TEST_CASE("upperCase", "StringTests")
{
    REQUIRE( toUpper("azAZ") == "AZAZ" );
    REQUIRE( toUpper("") == "" );
    REQUIRE( toUpper(" ") == " " );
}

TEST_CASE("format", "StringTests")
{
    REQUIRE( format("") == "" );
    REQUIRE( format("{") == "{" );
    REQUIRE( format("{}") == "" );
    REQUIRE( format("{}", 123) == "123" );
    REQUIRE( format("{} yo {}", 123, "456") == "123 yo 456" );
}
