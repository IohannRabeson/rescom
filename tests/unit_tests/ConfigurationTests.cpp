#include <Configuration.hpp>
#include <catch2/catch_all.hpp>

#include <FileSystem.hpp>
#include <ConfigurationParser.hpp>

std::vector<char> makeBuffer(std::string_view text)
{
    std::vector<char> result;

    result.resize(text.size());

    std::copy(text.begin(), text.end(), result.begin());

    return result;
}

Configuration parse(std::string const& text)
{
    auto filesystem = std::make_unique<InMemoryFileSystem>();
    filesystem->add("a.res", makeBuffer("hello world"));
    filesystem->add("b.res", makeBuffer("salut salut"));
    filesystem->add("a ab.res", makeBuffer("poulets"));

    ConfigurationParser parser{std::move(filesystem)};
    return parser.parseText(text, "memory");
}

TEST_CASE("empty", "ConfigurationTests") {

    auto configuration = parse("");

    CHECK( configuration.inputs.empty() );
}

TEST_CASE("empty spaces", "ConfigurationTests") {
    CHECK( parse("     ").inputs.empty() );
    CHECK( parse("\t").inputs.empty() );
    CHECK( parse("\n").inputs.empty() );
}

TEST_CASE("one line", "ConfigurationTests") {
    auto c = parse("a.res");

    REQUIRE( !c.inputs.empty() );
    CHECK( c.inputs.front().key == "a.res" );
}

TEST_CASE("one line with space", "ConfigurationTests") {
    auto c = parse("a ab.res");

    REQUIRE( !c.inputs.empty() );
    CHECK( c.inputs.front().key == "a ab.res" );
}

TEST_CASE("comment", "ConfigurationTests") {
    auto c = parse("#a.res\nb.res#yo");

    REQUIRE( c.inputs.size() == 1 );
    CHECK( c.inputs.front().key == "b.res" );
}

TEST_CASE("two line", "ConfigurationTests") {
    auto c = parse("a.res\nb.res");

    REQUIRE( c.inputs.size() == 2 );
    CHECK( c.inputs[0].key == "a.res" );
    CHECK( c.inputs[0].line == 1 );
    CHECK( c.inputs[1].key == "b.res" );
    CHECK( c.inputs[1].line == 2 );
}

