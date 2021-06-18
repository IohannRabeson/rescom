#include "ConfigurationParser.hpp"
#include "FileSystem.hpp"
#include "StringHelpers.hpp"

#include <string_view>
#include <fstream>
#include <filesystem>
#include <algorithm>

static constexpr char const* const OneLineCommentStart = "#";

namespace
{
    inline std::string_view cleanLine(std::string_view view, char const* oneLineCommentStart)
    {
        return trim(removeComment(view, oneLineCommentStart));
    }
}

ConfigurationParser::ConfigurationParser(std::unique_ptr<FileSystem>&& fileSystem)
: _fileSystem(std::move(fileSystem))
{
}

Configuration ConfigurationParser::parseFile(std::filesystem::path const& configurationFilePath) const
{
    std::ifstream file{configurationFilePath, std::ios::in};

    if (file.is_open())
    {
        return parse(file, configurationFilePath);
    }
    else
    {
        throw std::runtime_error(format("unable to open '{}' for reading", configurationFilePath.generic_string()));
    }
}

Configuration ConfigurationParser::parse(std::istream& stream, std::filesystem::path const& configurationFilePath) const
{
    std::string lineBuffer;
    std::vector<Input> inputs;
    std::size_t linePosition = 1u;

    while (std::getline(stream, lineBuffer))
    {
        auto const fileName = cleanLine(lineBuffer, OneLineCommentStart);

        if (!fileName.empty())
        {
            auto const configurationDirectory = configurationFilePath.parent_path();
            auto const absoluteInputPath{configurationDirectory / fileName};
            auto const relativeInputPath = std::filesystem::relative(absoluteInputPath, configurationDirectory);

            Input input{
                absoluteInputPath,
                relativeInputPath.generic_string(),
                _fileSystem->fileSize(absoluteInputPath),
                linePosition
            };

            checkResourceFile(input, configurationFilePath);

            inputs.emplace_back(std::move(input));
        }

        ++linePosition;
    }

    // We need to ensure the resources are ordered by the key otherwise
    // the generated access function rescom::getResource() will not work (it's a binary search).
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

/// Throws if the file specified by 'input' does not exists or is not a regular file.
void ConfigurationParser::checkResourceFile(Input const& input, std::filesystem::path const& configurationFilePath) const
{
    if (!_fileSystem->exists(input.filePath))
    {
        throw std::runtime_error(format("{}:{}: resource file not found '{}'\n -> file was expected to be here: {}", configurationFilePath.generic_string(), input.line, input.key, input.filePath.generic_string()));
    }

    if (!_fileSystem->exists(input.filePath))
    {
        throw std::runtime_error(format("{}:{}: '{}' is not a file", configurationFilePath.generic_string(), input.line, input.filePath.generic_string()));
    }
}

