#ifndef RESCOM_CONFIGURATION_HPP
#define RESCOM_CONFIGURATION_HPP
#include <filesystem>
#include <vector>
#include <string>
#include <optional>

struct Input
{
    std::filesystem::path filePath;
    std::string variable_name;
    std::string key;
    std::uint64_t size;
};

struct Configuration
{
    /// Path to the resource file path
    std::filesystem::path configurationFilePath;
    /// Informations readed from the resource file path.
    /// Those input are ordered by key.
    std::vector<Input> inputs;

    unsigned int tabulationSize = 4u;

    static Configuration fromFile(std::filesystem::path const& configurationFilePath, bool recurse);
};

#endif //RESCOM_CONFIGURATION_HPP
