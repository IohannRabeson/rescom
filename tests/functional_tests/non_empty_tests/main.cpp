#define CATCH_CONFIG_MAIN

#include <catch2/catch_all.hpp>
#include <rescom.hpp>
#include <filesystem>
#include <fstream>
#include <iterator>

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

std::vector<char> loadFile(std::filesystem::path const& path)
{
    std::ifstream file(path, std::ios::binary);
    auto const fileSize = std::filesystem::file_size(path);
    std::vector<char> buffer;

    buffer.resize(fileSize);
    file.read(buffer.data(), fileSize);

    return buffer;
}

void testContentIntegrity(std::string const& key)
{
    auto const& slot = rescom::getResource(key.c_str());
    std::filesystem::path const realFilePath = std::filesystem::path(RESOURCES_DIRECTORY) / key;

    std::vector<char> resourceBuffer(slot.bytes, slot.bytes + slot.size);
    std::vector<char> fileBuffer = loadFile(realFilePath);

    REQUIRE( resourceBuffer == fileBuffer );
}

TEST_CASE("content integrity", "[NonEmptyTests]") {
    for (auto it = rescom::begin(); it != rescom::end(); ++it)
        testContentIntegrity(it->key);
}
