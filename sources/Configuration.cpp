#include "Configuration.hpp"
#include "StringHelpers.hpp"
#include "RescomFileParser.hpp"

#include <fstream>
#include <iostream>
#include <cassert>
#include <stdexcept>
#include <stack>
#include <algorithm>

#include <fmt/core.h>

#include <lexy/dsl.hpp>



// static constexpr char const* const OneLineCommentStart = "#";
//
//namespace
//{
//    inline std::string_view cleanLine(std::string_view view, char const* oneLineCommentStart)
//    {
//        return trim(removeComment(view, oneLineCommentStart));
//    }
//}

Input createInput(std::filesystem::path const& configurationDirectory, std::filesystem::path const& absoluteInputPath, std::optional<std::string> const& key)
{
    auto finalKey = key.has_value() ? *key : std::filesystem::relative(absoluteInputPath, configurationDirectory).generic_string();

    return Input{
        absoluteInputPath,
        finalKey,
        std::filesystem::file_size(absoluteInputPath)
    };
}


Configuration Configuration::fromFile(std::filesystem::path const& configurationFilePath)
{
    std::ifstream configurationFile(configurationFilePath, std::ifstream::in);
    auto const configurationDirectory = configurationFilePath.parent_path();

    if (!configurationFile.is_open())
    {
        throw std::runtime_error(fmt::format("unable to read '{}'", configurationFilePath.generic_string()));
    }

    RescomFileParser parser(configurationDirectory);

    std::vector<std::pair<std::optional<std::string>, std::filesystem::path>> elements = parser.parseInputs(configurationFile);

    for (auto const& element : elements)
    {
        auto const& filePath = element.second;

        if (!std::filesystem::is_regular_file(filePath))
            throw std::runtime_error(fmt::format("{}: '{}' is not a file", configurationFilePath.generic_string(), filePath.generic_string()));
        if (!std::filesystem::exists(filePath))
            throw std::runtime_error(fmt::format("{}: '{}' does not exists", configurationFilePath.generic_string(), filePath.generic_string()));
    }

    std::vector<Input> inputs;

    inputs.reserve(elements.size());

    for (auto const& element : elements)
    {
        inputs.emplace_back(createInput(configurationDirectory, element.second, element.first));
    }

//    std::size_t linePosition = 1u;
//
//    while (std::getline(configurationFile, lineBuffer))
//    {
//        auto const fileName = cleanLine(lineBuffer, OneLineCommentStart);
//
//        if (!fileName.empty())
//        {
//            createInputs(configurationFilePath, linePosition, fileName, recurse, inputs);
//        }
//
//        ++linePosition;
//    }

    // We need to ensure the resources are ordered by the key otherwise
    // the access function (rescom::getResource) will not work.
    std::sort(inputs.begin(), inputs.end(), [](Input const& left, Input const& right)
    {
        return left.key < right.key;
    });

    return Configuration { configurationFilePath, std::move(inputs) };
}
