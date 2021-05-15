#include "Configuration.hpp"
#include "Strings.hpp"

#include <fstream>
#include <iostream>
#include <cassert>

namespace
{
    std::string generateKey(std::filesystem::path const& inputFilePath)
    {
        assert(inputFilePath.is_relative());
        auto path = inputFilePath.generic_string();
        replaceAll(path, std::filesystem::path::preferred_separator, '_');
        replaceAll(path, '.', '_');
        return path;
    }
}

std::optional<Configuration> Configuration::fromFile(std::filesystem::path const& configurationFilePath)
{
    std::ifstream configurationFile(configurationFilePath, std::ifstream::in);
    std::string lineBuffer;
    std::vector<Input> inputs;

    if (!configurationFile.is_open())
    {
        std::cerr << "Unable to open configuration file '" << configurationFilePath << "'\n";
        return {};
    }

    while (std::getline(configurationFile, lineBuffer))
    {
        auto const configurationDirectory = configurationFilePath.parent_path();
        auto const absoluteInputPath{configurationDirectory / lineBuffer};

        if (!std::filesystem::exists(absoluteInputPath))
        {
            std::cerr << "File '" << absoluteInputPath << "'" << " not found.\n";
            continue;
        }

        if (std::filesystem::is_regular_file(absoluteInputPath))
        {
            auto relativeInputPath = std::filesystem::relative(absoluteInputPath, configurationDirectory);

            inputs.emplace_back(Input {
                .filePath = absoluteInputPath,
                .variable_name = generateKey(relativeInputPath),
                .key = relativeInputPath.generic_string(),
                .size = std::filesystem::file_size(absoluteInputPath),
            });
        }
        else
        {
            std::cerr << "'" << lineBuffer << "'" << " is not a file.\n";
            continue;
        }
    }

    // We need to ensure the resources are ordered by the key otherwise
    // the access function (rescom::getResource) will not work.
    std::sort(inputs.begin(), inputs.end(), [](Input const& left, Input const& right)
    {
        return left.key < right.key;
    });

    return Configuration { .configurationFilePath = configurationFilePath, .inputs = std::move(inputs) };
}