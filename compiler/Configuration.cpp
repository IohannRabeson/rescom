#include "Configuration.hpp"
#include "Strings.hpp"

#include <fstream>
#include <iostream>
#include <cassert>
#include <stdexcept>

#include <fmt/core.h>

static constexpr char const* const OneLineCommentStart = "#";

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

inline std::string_view cleanLine(std::string_view view, char const* oneLineCommentStart)
{
    return trim(removeComment(view, oneLineCommentStart));
}

Configuration Configuration::fromFile(std::filesystem::path const& configurationFilePath)
{
    std::ifstream configurationFile(configurationFilePath, std::ifstream::in);
    std::string lineBuffer;
    std::vector<Input> inputs;

    if (!configurationFile.is_open())
    {
        throw std::runtime_error(fmt::format("unable to read '{}'", configurationFilePath.generic_string()));
    }

    std::size_t linePosition = 1u;

    while (std::getline(configurationFile, lineBuffer))
    {
        auto const fileName = cleanLine(lineBuffer, OneLineCommentStart);

        if (fileName.starts_with(OneLineCommentStart))
            continue;

        auto const configurationDirectory = configurationFilePath.parent_path();
        auto const absoluteInputPath{configurationDirectory / fileName};

        if (!std::filesystem::exists(absoluteInputPath))
        {
            throw std::runtime_error(fmt::format("{}:{}: resource file not found '{}'\n -> file was expected to be here: {}", configurationFilePath.generic_string(), linePosition, fileName, absoluteInputPath.generic_string()));
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
            // TODO support directories
            std::cerr << "'" << lineBuffer << "'" << " is not a file.\n";
            continue;
        }

        ++linePosition;
    }

    // We need to ensure the resources are ordered by the key otherwise
    // the access function (rescom::getResource) will not work.
    std::sort(inputs.begin(), inputs.end(), [](Input const& left, Input const& right)
    {
        return left.key < right.key;
    });

    return Configuration { .configurationFilePath = configurationFilePath, .inputs = std::move(inputs) };
}