#include "CodeGenerator.hpp"
#include "Configuration.hpp"
#include "StringHelpers.hpp"

#include <filesystem>
#include <fstream>
#include <iterator>
#include <iostream>
#include <stdexcept>

#include <fmt/core.h>

namespace
{
    static constexpr char const* NamespaceForResourceData = "rescom";

    void loadFile(std::filesystem::path const& filePath, std::vector<char>& buffer)
    {
        std::ifstream file{filePath, std::ios::binary};

        if (file.is_open())
        {
            buffer.assign(std::istreambuf_iterator<char>(file), {});
        }
        else
        {
            throw std::runtime_error(fmt::format("unable to read '{}'", filePath.generic_string()));
        }
    }
}

static std::string const HeaderProtectionMacroPrefix = "RESCOM_GENERATED_FILE_";

CodeGenerator::CodeGenerator(Configuration const& configuration)
: _configuration(configuration)
, _tabulation(configuration.tabulationSize, ' ')
, _headerProtectionMacroName(HeaderProtectionMacroPrefix + toUpper(_configuration.configurationFilePath.stem().generic_string()))
{
}

std::string CodeGenerator::tab(unsigned int count) const
{
    if (count == 0)
        return {};

    std::string result;

    result.reserve(count * _tabulation.size());

    for (auto i = 0u; i < count; ++i)
        result += _tabulation;

    return result;
}

CompilationResult CodeGenerator::compile(std::ostream& output)
{
    writeFileHeader(output);
    writeResources(output);
    writeAccessFunction(output);
    writeFileFooter(output);

    return CompilationResult::Ok;
}

void CodeGenerator::writeFileHeader(std::ostream& output)
{
    static std::string const Includes[] = {
        "<stdexcept>",
        "<iterator>",
        "<string_view>",
        "<cstring>" // for std::strcmp
    };
    auto resourceFileStem = toUpper(_configuration.configurationFilePath.stem().generic_string());

    output << "// Generated by Rescom\n";
    output << fmt::format("#ifndef {0}\n#define {0}\n", _headerProtectionMacroName);

    for (auto const& include : Includes)
        output << fmt::format("#include {}\n", include);
    output << "\n";

    output << tab(0) << "namespace " << NamespaceForResourceData << "\n{\n";
    output << tab(1) << "struct Resource\n"
           << tab(1) << "{\n"
           << tab(2) << "char const* const key;\n"
           << tab(2) << "char const* const bytes;\n"
           << tab(2) << "unsigned int const size;\n"
           << "\n"
           << tab(2) << "constexpr Resource(char const* key, unsigned int size, char const* bytes)\n"
           << tab(2) << ": key(key), bytes(bytes), size(size) {}\n"
           << tab(1) << "};\n\n";
}

void CodeGenerator::writeFileFooter(std::ostream& output)
{
    auto resourceFileStem = toUpper(_configuration.configurationFilePath.stem().generic_string());

    output << tab(0) << "} // namespace " << NamespaceForResourceData << "\n";
    output << "#endif // " << _headerProtectionMacroName << "\n";
}

void CodeGenerator::writeResource(Input const& input, std::vector<char> const& bytes, std::ostream& output)
{
    output << tab(3) << "{\"" << input.key << "\", " << input.size << ", " << "\"";

    output << std::hex;

    for (auto const c : bytes) {
        // Ensure the value printed is never negative by casting the byte to unsigned char
        // then convert to unsigned int to ensure operator << will print a hexadecimal number
        // and not a character.
        output << "\\x" << static_cast<unsigned int>(static_cast<unsigned char>(c));
    }

    output << std::dec;
    output << "\" },\n";
}

/// Write the code to access to a specific resource.
/// The generated code uses the fact resources are ordered by their key to use a constexpr version of std::lower_bound and
///// keep the compilation time acceptable.
void CodeGenerator::writeAccessFunction(std::ostream& output)
{
    output << tab(1) << "namespace details {\n";
    if (!_configuration.inputs.empty()) {
        output << tab(2) << "constexpr bool compareSlot(Resource const& slot, char const * key) { return std::string_view(slot.key) < key; }\n\n";
        // Function lowerBound is an ugly copy-paste of https://en.cppreference.com/w/cpp/algorithm/lower_bound.
        // But I don't care, it's constexpr and it just works.
        output << tab(2) << "template<class ForwardIt, class Compare>\n"
               << tab(2) << "constexpr ForwardIt lowerBound(ForwardIt first, ForwardIt last, char const* value, Compare compare)\n"
               << tab(2) << "{\n"
               << tab(3) << "if (value == nullptr) return last;\n"
               << tab(3) << "typename std::iterator_traits<ForwardIt>::difference_type count;\n"
               << tab(3) << "typename std::iterator_traits<ForwardIt>::difference_type step;\n"
               << tab(3) << "count = std::distance(first, last);\n"
               << tab(3) << "ForwardIt it;\n"
               << tab(3) << "while (count > 0u) {\n"
               << tab(4) << "it = first; step = count / 2; std::advance(it, step);\n"
               << tab(4) << "if (compare(*it, value)) { first = ++it; count -= step + 1; } else { count = step; }\n"
               << tab(3) << "}\n"
               << tab(3) << "return first->key != nullptr && std::strcmp(value, first->key) == 0 ? first : last;\n"
               << tab(2) << "}\n";
    }

    output << tab(2) << "static constexpr Resource const NullResource{nullptr, 0u, nullptr};\n";

    output << tab(1) << "} // namespace details\n\n";

    if (_configuration.inputs.empty())
    {
        output << tab() << "inline constexpr Resource const& getResource(char const*)\n"
               << tab() << "{\n"
               << tab(2) << "return details::NullResource;\n"
               << tab() << "}\n";    }
    else
    {
        output << tab() << "inline constexpr Resource const& getResource(char const* key)\n"
               << tab() << "{\n"
               << tab(2) << "auto it = details::lowerBound(std::begin(details::Slots), std::end(details::Slots), key, details::compareSlot);\n"
               << "\n"
               << tab(2) << "if (it == std::end(details::Slots))\n"
               << tab(3) << "return details::NullResource;\n"
               << "\n"
               << tab(2) << "return *it;\n"
               << tab() << "}\n";
    }

    output << tab() << "inline constexpr bool contains(char const* key)\n"
           << tab() << "{\n"
           << tab(2) << "return &getResource(key) != &details::NullResource;\n"
           << tab() << "}\n";

    output << "\n"
           << tab() << "inline constexpr std::string_view getText(char const* key)\n"
           << tab() << "{\n"
           << tab(2) << "auto const& resource = getResource(key);\n"
           << "\n"
           << tab(2) << "return std::string_view{resource.bytes, resource.size};\n"
           << tab() << "}\n";

    output << "\n"
           << tab() << "inline constexpr Resource const* begin()\n"
           << tab() << "{\n";
    if (_configuration.inputs.empty())
    {
        output << tab(2) << "return &details::NullResource;\n";
    }
    else
    {
        output << tab(2) << "return std::begin(details::Slots);\n";
    }
    output << tab() << "}\n";

    output << "\n"
           << tab() << "inline constexpr Resource const* end()\n"
           << tab() << "{\n";
    if (_configuration.inputs.empty())
    {
        output << tab(2) << "return &details::NullResource;\n";
    }
    else
    {
        output << tab(2) << "return std::end(details::Slots);\n";
    }
    output << tab() << "}\n";
}

CompilationResult CodeGenerator::writeResources(std::ostream& output)
{
    if (_configuration.inputs.empty())
        return CompilationResult::Ok;

    std::vector<char> buffer;

    buffer.reserve(1024 * 16);

    output << tab(1) << "namespace details {\n";
    output << tab(2) << "constexpr unsigned int const ResourcesCount = " << _configuration.inputs.size() << ";\n";
    output << tab(2) << "constexpr Resource const Slots[ResourcesCount] = \n";
    output << tab(2) << "{\n";
    for (auto const& input : _configuration.inputs)
    {
        loadFile(input.filePath, buffer);
        writeResource(input, buffer, output);
    }

    output << tab(2) << "};\n";
    output << tab(1) << "} // namespace details\n";

    return CompilationResult::Ok;
}
