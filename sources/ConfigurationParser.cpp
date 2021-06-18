#include "ConfigurationParser.hpp"
#include "FileSystem.hpp"
#include "StringHelpers.hpp"

#include <string_view>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <sstream>

namespace
{
    static constexpr char const* const OneLineCommentStart = "#";

    inline std::string_view cleanLine(std::string_view view, char const* oneLineCommentStart)
    {
        return trim(removeComment(view, oneLineCommentStart));
    }

    /// Throws std::runtime_error if the file specified by 'input' does not exists or is not a regular file.
    void checkResourceFile(std::unique_ptr<FileSystem> const& fileSystem, Input const& input,
                           std::filesystem::path const& configurationFilePath)
    {
        if (!fileSystem->exists(input.filePath))
        {
            throw std::runtime_error(format("{}:{}: resource file not found '{}'\n -> file was expected to be here: {}", configurationFilePath.generic_string(), input.line, input.key, input.filePath.generic_string()));
        }

        if (!fileSystem->exists(input.filePath))
        {
            throw std::runtime_error(format("{}:{}: '{}' is not a file", configurationFilePath.generic_string(), input.line, input.filePath.generic_string()));
        }
    }

    Configuration parseConfiguration(std::unique_ptr<FileSystem> const& fileSystem, std::istream& stream,
                                     std::filesystem::path const& configurationFilePath)
    {
        std::string lineBuffer;
        size_t linePosition = 1u;
        std::vector<Input> inputs;

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
                    fileSystem->fileSize(absoluteInputPath),
                    linePosition
                };

                checkResourceFile(fileSystem, input, configurationFilePath);

                inputs.emplace_back(std::move(input));
            }

            ++linePosition;
        }

        return Configuration{configurationFilePath, inputs};
    }
}

ConfigurationParser::ConfigurationParser(std::unique_ptr<FileSystem>&& fileSystem)
: _fileSystem(std::move(fileSystem))
{
}

Configuration ConfigurationParser::parseText(std::string const& text, std::filesystem::path const& configurationFilePath) const
{
    std::istringstream iss{text};

    return parseStream(iss, configurationFilePath);
}

Configuration ConfigurationParser::parseFile(std::filesystem::path const& configurationFilePath) const
{
    std::ifstream file{configurationFilePath, std::ios::in};

    if (file.is_open())
    {
        return parseStream(file, configurationFilePath);
    }
    else
    {
        throw std::runtime_error(format("unable to open '{}' for reading", configurationFilePath.generic_string()));
    }
}

Configuration ConfigurationParser::parseStream(std::istream& stream, std::filesystem::path const& configurationFilePath) const
{
    auto configuration = parseConfiguration(_fileSystem, stream, configurationFilePath);

    // We need to ensure the resources are ordered by the key otherwise
    // the generated access function rescom::getResource() will not work (it's a binary search).
    std::sort(configuration.inputs.begin(), configuration.inputs.end(), [](Input const& left, Input const& right)
    {
        return left.key < right.key;
    });

    // Ensure unicity using the field 'key'
    configuration.inputs.erase(std::unique(configuration.inputs.begin(), configuration.inputs.end(),
                                           [](Input const& left, Input const& right) { return left.key == right.key; }),
                               configuration.inputs.end());

    return configuration;
}