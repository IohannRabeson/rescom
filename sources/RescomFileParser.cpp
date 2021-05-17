#include "RescomFileParser.hpp"
#include "StringHelpers.hpp"

#include <lexy/input/string_input.hpp>
#include <lexy_ext/report_error.hpp>
#include <lexy/parse.hpp>

#include <cassert>
#include <iostream>
#include <string_view>

static constexpr char const* const OneLineCommentStart = "#";

namespace
{
    inline std::string_view cleanLine(std::string_view view, char const* oneLineCommentStart)
    {
        return trim(removeComment(view, oneLineCommentStart));
    }
}

std::vector<std::pair<std::optional<std::string>, std::filesystem::path>> RescomFileParser::parseInputs(std::istream& stream) const
{
    std::vector<std::pair<std::optional<std::string>, std::filesystem::path>> results;
    std::string buffer;

    while (std::getline(stream, buffer))
    {
        auto cleanedBuffer = cleanLine(buffer, OneLineCommentStart);

        if (cleanedBuffer.empty())
            continue;

        auto parseResult = lexy::parse<grammar::Input>(lexy::string_input(cleanedBuffer), lexy_ext::report_error);
        auto elements = parseResult.value();
        std::pair<std::optional<std::string>, std::filesystem::path> result;

        switch (elements.size())
        {
            case 1u:
                result.second = _configurationDirectory / std::filesystem::path{elements[0]};
                break;
            case 2u:
                result.first = elements[0];
                result.second = _configurationDirectory / std::filesystem::path{elements[1]};
                break;
        }

        results.emplace_back(result);
    }
    return results;
}

RescomFileParser::RescomFileParser(std::filesystem::path const& configurationDirectory)
: _configurationDirectory(configurationDirectory)
{
    assert( std::filesystem::is_directory(_configurationDirectory) );
}
