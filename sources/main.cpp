#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include <cxxopts.hpp>

#include <picosha2.h>

#include "Configuration.hpp"
#include "LegacyCppCodeGenerator.hpp"
#include "GeneratedConstants.hpp"
#include "StringHelpers.hpp"
#include "ConfigurationParser.hpp"
#include "FileSystem.hpp"

void releaseResults(cxxopts::ParseResult const& parseResult, std::ostringstream const& outputStream);

std::ostream& selectOutputStream(std::ofstream& file, std::ostream& fallback)
{
    return file.is_open() ? file : fallback;
}

void registerCodeGenerators()
{
    registerCodeGenerator("legacy", [](Configuration const& configuration){ return std::make_unique<LegacyCppCodeGenerator>(configuration); }, true);
}

CodeGeneratorPointer createGenerator(cxxopts::ParseResult const& parseResult, Configuration const& configuration)
{
    if (parseResult["generator"].count() == 0)
        return instanciateDefaultCodeGenerator(configuration);
    else
        return instanciateCodeGenerator(parseResult["generator"].as<std::string>(), configuration);
}

std::optional<std::filesystem::path> getFilePath(cxxopts::ParseResult const& parseResults, std::string const& key)
{
    if (parseResults.count(key) > 0)
        return parseResults[key].as<std::string>();
    return {};
}

std::vector<std::string> computeInputHashes(Configuration const& configuration)
{
    std::vector<std::string> actualHashes;

    for (auto const& input : configuration.inputs)
    {
        std::ifstream ifstream(input.filePath, std::ios::in);

        if (!ifstream.is_open())
            throw std::runtime_error("Unable to open input file for reading '" + input.filePath.generic_string() + "'");

        std::vector<char> buffer;

        std::copy(std::istream_iterator<char>(ifstream), std::istream_iterator<char>{}, std::back_inserter(buffer));
        actualHashes.push_back(picosha2::hash256_hex_string(buffer));
    }

    return actualHashes;
}

std::vector<std::string> readHashesFile(std::filesystem::path const& filePath)
{
    std::ifstream file{filePath, std::ios::in};

    if (!file.is_open())
        throw std::runtime_error("unable to read file '" + filePath.generic_string() + "'");

    std::istream_iterator<std::string> inputIt{file};
    std::istream_iterator<std::string> endInputIt;
    std::vector<std::string> lines;

    std::copy(inputIt, endInputIt, std::back_inserter(lines));

    return lines;
}

void writeHashesFile(std::filesystem::path const& filePath, std::vector<std::string> const& hashes)
{
    std::ofstream file{filePath, std::ios::out | std::ios::trunc};

    if (!file.is_open())
        throw std::runtime_error("unable to write file '" + filePath.generic_string() + "'");

    std::copy(hashes.begin(), hashes.end(), std::ostream_iterator<std::string>(file, "\n"));
}

bool skip(Configuration const& configuration, std::filesystem::path const& witnessFilePath)
{
    auto previousHashes = readHashesFile(witnessFilePath);

    std::vector<std::string> const actualHashes = computeInputHashes(configuration);

    writeHashesFile(witnessFilePath, actualHashes);

    return previousHashes == actualHashes;
}

int main(int argc, char** argv)
{
    try
    {
        registerCodeGenerators();

        std::vector<std::string> inputs;
        cxxopts::Options options("rescom", "Resources compiler");

        options.add_options()
            ("i,input", "Input file", cxxopts::value<std::string>())
            ("o,output", "Output file", cxxopts::value<std::string>())
            ("G,generator", "Generator", cxxopts::value<std::string>())
            ("version", "Print version", cxxopts::value<bool>())
            ("witness", "Witness file", cxxopts::value<std::string>())
            ;

        auto parseResult = options.parse(argc, argv);

        if (parseResult["version"].count() > 0)
        {
            std::cout << "rescom version " << RescomVersion << "\n";
            return 0;
        }

        std::filesystem::path const inputFilePath{parseResult["input"].as<std::string>()};
        std::optional<std::filesystem::path> witnessFilePath{getFilePath(parseResult, "witness")};
        std::ostringstream outputStream;

        ConfigurationParser parser{std::make_unique<LocalFileSystem>()};
        auto configuration = parser.parseFile(inputFilePath);

        if (witnessFilePath.has_value() && skip(configuration, *witnessFilePath))
        {
            std::cout << "Nothing changed\n";
            return 0;
        }

        auto generator = createGenerator(parseResult, configuration);

        if (generator != nullptr)
            generator->generate(outputStream);

        releaseResults(parseResult, outputStream);
    }
    catch (std::exception const& error)
    {
        std::cerr << "Rescom error: " << error.what() << "\n";

        return 1;
    }

    return 0;
}

void releaseResults(cxxopts::ParseResult const& parseResult, std::ostringstream const& outputStream)
{
    if (parseResult.count("output") > 0)
    {
        std::filesystem::path const outputFilePath{parseResult["output"].as<std::string>()};
        std::ofstream outputFile(outputFilePath, std::ios::out | std::ios::ate);

        if (!outputFile.is_open())
            throw std::runtime_error(format("unable to open '{}' for writing", outputFilePath.generic_string()));

        outputFile << outputStream.str();
    }
    else
    {
        std::cout << outputStream.str();
    }
}

