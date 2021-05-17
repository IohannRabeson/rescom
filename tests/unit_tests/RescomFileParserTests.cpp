#include <catch2/catch_all.hpp>

#include "RescomFileParser.hpp"

#include <lexy/parse.hpp>
#include <lexy/input/string_input.hpp>
#include <lexy_ext/report_error.hpp>

template <typename PRODUCTION>
void testParseSimpleString(std::string const& input, std::string const& expected)
{
    auto result = lexy::parse<PRODUCTION>(lexy::string_input(input), lexy_ext::report_error);

    REQUIRE( result.value() == expected );
}

template <typename PRODUCTION>
void testParseStrings(std::string const& input, std::vector<std::string> const& expected)
{
    auto result = lexy::parse<PRODUCTION>(lexy::string_input(input), lexy_ext::report_error);

    REQUIRE( result.value() == expected );
}

TEST_CASE("element", "ConfigurationParserDev") {
    testParseSimpleString<grammar::UnquotedElement>("D:/a/b.c", "D:/a/b.c");
    testParseSimpleString<grammar::UnquotedElement>("\"a", "\"a");
    testParseSimpleString<grammar::UnquotedElement>("D:\\a\\b.c", "D:\\a\\b.c");
    testParseSimpleString<grammar::QuotedElement>("\"D:/a/b.c\"", "D:/a/b.c");
    testParseSimpleString<grammar::QuotedElement>("\"D:\\a\\b.c\"", "D:\\a\\b.c");
    testParseSimpleString<grammar::Element>("\"D:\\a\\b.c\"", "D:\\a\\b.c");
    testParseSimpleString<grammar::Element>("D:\\a\\b.c", "D:\\a\\b.c");
    testParseSimpleString<grammar::Element>("\"a", "\"a");
}

TEST_CASE("input", "ConfigurationParserDev") {
    testParseStrings<grammar::Input>("key", {"key"});
    testParseStrings<grammar::Input>("key;yo", {"key", "yo"});
    testParseStrings<grammar::Input>("\"key\";yo", {"key", "yo"});
    testParseStrings<grammar::Input>("key;\"yo\"", {"key", "yo"});
    testParseStrings<grammar::Input>("\"key\";\"yo\"", {"key", "yo"});
    testParseStrings<grammar::Input>("\"key;yo\"", {"key;yo"});
    testParseStrings<grammar::Input>("\"key;yo", {"\"key", "yo"});
    testParseStrings<grammar::Input>("\"key", {"\"key"});
}

