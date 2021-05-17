#ifndef RESCOM_RESCOMFILEPARSER_HPP
#define RESCOM_RESCOMFILEPARSER_HPP
#include "Configuration.hpp"

#include <vector>
#include <string>
#include <filesystem>

#include <lexy/dsl.hpp>

namespace grammar
{
    namespace dsl = lexy::dsl;

    struct UnquotedElement
    {
        static constexpr auto rule = dsl::capture(dsl::while_(dsl::ascii::character - dsl::lit_c<';'>));

        static constexpr auto value = lexy::as_string<std::string>;
    };

    struct QuotedElement
    {
        static constexpr auto rule = dsl::quoted(dsl::ascii::character);

        static constexpr auto value = lexy::as_string<std::string>;
    };

    struct Element
    {
        static constexpr auto rule = lexy::dsl::peek(dsl::p<QuotedElement>) >> dsl::p<QuotedElement> | dsl::else_ >> dsl::p<UnquotedElement>;

        static constexpr auto value = lexy::as_string<std::string>;
    };

    struct Input
    {
        static constexpr auto whitespace = dsl::ascii::space;

        static constexpr auto rule = dsl::list(dsl::p<Element>, dsl::sep(dsl::semicolon));

        static constexpr auto value = lexy::as_list<std::vector<std::string>>;
    };
}

class RescomFileParser
{
    std::filesystem::path const _configurationDirectory;
public:
    struct Entry
    {
        std::optional<std::string> key;
        std::filesystem::path filePath;
    };

    explicit RescomFileParser(std::filesystem::path const& configurationDirectory);

    std::vector<Entry> parseInputs(std::istream& stream) const;
};

#endif //RESCOM_RESCOMFILEPARSER_HPP
