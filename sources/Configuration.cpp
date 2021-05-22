#include "Configuration.hpp"
#include "StringHelpers.hpp"

#include <fstream>
#include <iostream>
#include <cassert>
#include <stdexcept>
#include <stack>
#include <algorithm>

static constexpr char const* const OneLineCommentStart = "#";

namespace
{
    inline std::string_view cleanLine(std::string_view view, char const* oneLineCommentStart)
    {
        return trim(removeComment(view, oneLineCommentStart));
    }
}

Input createInput(std::filesystem::path const& configurationDirectory, std::filesystem::path const& absoluteInputPath)
{
    auto relativeInputPath = std::filesystem::relative(absoluteInputPath, configurationDirectory);

    return Input{
        absoluteInputPath,
        relativeInputPath.generic_string(),
        std::filesystem::file_size(absoluteInputPath)
    };
}

void createInputs(std::filesystem::path const& configurationFilePath, size_t linePosition, std::string_view const& fileName, std::vector<Input>& results)
{
    auto const configurationDirectory = configurationFilePath.parent_path();
    auto const absoluteInputPath{configurationDirectory / fileName};

    if (!std::filesystem::exists(absoluteInputPath))
    {
        throw std::runtime_error(format("{}:{}: resource file not found '{}'\n -> file was expected to be here: {}", configurationFilePath.generic_string(), linePosition, fileName, absoluteInputPath.generic_string()));
    }

    if (std::filesystem::is_regular_file(absoluteInputPath))
    {
        results.emplace_back(createInput(configurationDirectory, absoluteInputPath));
    }
    else
    {
        throw std::runtime_error(format("{}:{}: '{}' is not a file", configurationFilePath.generic_string(), linePosition, absoluteInputPath.generic_string()));
    }
}

Configuration Configuration::fromFile(std::filesystem::path const& configurationFilePath)
{
    std::ifstream configurationFile(configurationFilePath, std::ifstream::in);
    std::string lineBuffer;
    std::vector<Input> inputs;

    if (!configurationFile.is_open())
        throw std::runtime_error(format("unable to read '{}'", configurationFilePath.generic_string()));

    return fromStream(configurationFile, configurationFilePath);
}

Configuration Configuration::fromStream(std::istream& stream, std::filesystem::path const& configurationFilePath)
{
    std::string lineBuffer;
    std::vector<Input> inputs;
    std::size_t linePosition = 1u;

    while (std::getline(stream, lineBuffer))
    {
        auto const fileName = cleanLine(lineBuffer, OneLineCommentStart);

        if (!fileName.empty())
        {
            createInputs(configurationFilePath, linePosition, fileName, inputs);
        }

        ++linePosition;
    }

    // We need to ensure the resources are ordered by the key otherwise
    // the access function (rescom::getResource) will not work.
    std::sort(inputs.begin(), inputs.end(), [](Input const& left, Input const& right)
    {
        return left.key < right.key;
    });

    // Ensure unique keys
    inputs.erase(std::unique(inputs.begin(), inputs.end(), [](Input const& left, Input const& right)
    {
        return left.key == right.key;
    }), inputs.end());

    return Configuration { configurationFilePath, std::move(inputs) };
}
