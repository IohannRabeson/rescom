#ifndef RESCOM_CONFIGURATIONPARSER_HPP
#define RESCOM_CONFIGURATIONPARSER_HPP
#include "Configuration.hpp"

#include <memory>
#include <istream>
#include <filesystem>

class FileSystem;

class ConfigurationParser
{
    std::unique_ptr<FileSystem> const _fileSystem;
public:
    explicit ConfigurationParser(std::unique_ptr<FileSystem>&& fileSystem);

    Configuration parse(std::istream& stream, std::filesystem::path const& configurationFilePath) const;
    Configuration parseFile(std::filesystem::path const& configurationFilePath) const;
private:
    void checkResourceFile(Input const& input, std::filesystem::path const& configurationFilePath) const;
};

#endif //RESCOM_CONFIGURATIONPARSER_HPP
