#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include <cxxopts.hpp>

#include "Configuration.hpp"
#include "Compiler.hpp"

std::ostream& selectOutputStream(std::ofstream& file, std::ostream& fallback)
{
    return file.is_open() ? file : fallback;
}

int main(int argc, char** argv)
{
    std::vector<std::string> inputs;
    cxxopts::Options options("rescom", "Resources compiler");

    options.add_options()
        ("i,input", "Input file", cxxopts::value<std::string>())
        ("o,output", "Output file", cxxopts::value<std::string>())
        ;

    auto parseResult = options.parse(argc, argv);

    std::filesystem::path const outputFilePath{parseResult.count("output") > 0 ? parseResult["output"].as<std::string>() : ""};
    std::filesystem::path const inputFilePath{parseResult["input"].as<std::string>()};
    std::ofstream outputFile{outputFilePath, std::ios::out};

    if (auto configuration = Configuration::fromFile(inputFilePath); configuration.has_value())
    {
        Compiler c(configuration.value());

        switch (c.compile(selectOutputStream(outputFile, std::cout)))
        {
            case CompilationResult::Ok: break;
            case CompilationResult::Error: break;
        }
    }

    return 0;
}

