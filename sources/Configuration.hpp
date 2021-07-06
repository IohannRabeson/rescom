#ifndef RESCOM_CONFIGURATION_HPP
#define RESCOM_CONFIGURATION_HPP
#include <filesystem>
#include <string>
#include <cstdint>
#include <vector>
#include "LegacyCppCodeGenerator.hpp"

struct Input
{
    /// The absolute path of the input file
    std::filesystem::path filePath;
    /// The key of the resource
    std::string key;
    /// The size of the resource in bytes
    std::uint64_t size;
    /// The line where this input has been parsed
    std::size_t line;
};

/// Defines files to embed as resources.
/// Also specifies how the C++ code is generated.
struct Configuration
{
    /// Path to the configuration file path.
    std::filesystem::path configurationFilePath;

    /// Informations readed from the resource file path.
    /// Those inputs are ordered by key.
    std::vector<Input> inputs;

    /// Tabulation size of the generated code.
    unsigned int tabulationSize = 4u;
};

#endif //RESCOM_CONFIGURATION_HPP
