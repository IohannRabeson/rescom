#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include <cxxopts.hpp>

struct ResourcesConfiguration
{
    std::vector<std::filesystem::path> inputs;
};

std::optional<ResourcesConfiguration> readFromFile(std::filesystem::path const& resourcesConfigurationfilePath)
{
    std::ifstream ifstream(resourcesConfigurationfilePath, std::ifstream::in);
    std::string lineBuffer;
    ResourcesConfiguration result;    

    if (!ifstream.is_open())
        return {};

    while (std::getline(ifstream, lineBuffer))
    {
        std::filesystem::path inputFilePath{lineBuffer};

        if (!std::filesystem::exists(inputFilePath))
        {
            std::cerr << "'" << lineBuffer << "'" << " doesn't exists.\n";
            continue;
        }

        if (std::filesystem::is_regular_file(inputFilePath)) 
        {
            result.inputs.emplace_back(inputFilePath);
        }
        else
        {
            std::cerr << "'" << lineBuffer << "'" << " is not a file.\n";
            continue;
        }
    }

    return result;
}

int main(int argc, char** argv)
{
    std::vector<std::string> inputs;
    std::string const InputOptionName = "inputs";
    cxxopts::Options options("rescom", "Resources compiler");

    options.add_options()
        (InputOptionName, "Input files", cxxopts::value<std::vector<std::string>>(inputs));

    std::vector<std::string> const positionalOptionNames = {InputOptionName};
    options.parse_positional(positionalOptionNames.begin(), positionalOptionNames.end());

    auto result = options.parse(argc, argv);

    for (auto const& input : inputs)
    {
        if (auto config = readFromFile(std::filesystem::path{input}); config) 
        {
            for (auto i = 0u; i < config->inputs.size(); ++i)
            {
                std::cout << " - " << config->inputs[i].string() << "\n";
            }
        }
    }

    return 0;
}
