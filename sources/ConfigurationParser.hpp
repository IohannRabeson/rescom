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

    Configuration parseStream(std::istream& stream, std::filesystem::path const& configurationFilePath) const;
    Configuration parseText(std::string const& text, std::filesystem::path const& configurationFilePath) const;
    Configuration parseFile(std::filesystem::path const& configurationFilePath) const;
};

#endif //RESCOM_CONFIGURATIONPARSER_HPP
