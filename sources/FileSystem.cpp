#include "FileSystem.hpp"
#include "StringHelpers.hpp"
#include <fstream>
#include <iterator>

//
// class LocalFileSystem
//
std::uint64_t LocalFileSystem::fileSize(std::filesystem::path const& path) const
{
    return std::filesystem::file_size(path);
}

bool LocalFileSystem::exists(std::filesystem::path const& path) const
{
    return std::filesystem::exists(path);
}

bool LocalFileSystem::isRegularFile(std::filesystem::path const& path) const
{
    return std::filesystem::is_regular_file(path);
}

void LocalFileSystem::getContent(std::filesystem::path const& path, std::vector<char>& buffer) const
{
    std::ifstream file{path, std::ios::binary};

    if (file.is_open())
    {
        buffer.assign(std::istreambuf_iterator<char>(file), {});
    }
    else
    {
        throw std::runtime_error(format("unable to read '{}'", path.generic_string()));
    }
}

//
// class InMemoryFileSystem
//
std::uint64_t InMemoryFileSystem::fileSize(std::filesystem::path const& path) const
{
    if (auto it = _files.find(path); it != _files.end())
    {
        return it->second.buffer.size();
    }

    throw std::range_error(format("file '{}' not found", path.generic_string()));
}

bool InMemoryFileSystem::exists(std::filesystem::path const& path) const
{
    if (auto it = _files.find(path); it != _files.end())
    {
        return it->second.exists;
    }

    throw std::range_error(format("file '{}' not found", path.generic_string()));
}

bool InMemoryFileSystem::isRegularFile(std::filesystem::path const& path) const
{
    if (auto it = _files.find(path); it != _files.end())
    {
        return it->second.isRegularFile;
    }

    throw std::range_error(format("file '{}' not found", path.generic_string()));
}

void InMemoryFileSystem::getContent(std::filesystem::path const& path, std::vector<char>& buffer) const
{
    if (auto it = _files.find(path); it != _files.end())
    {
        buffer = it->second.buffer;
    }

    throw std::range_error(format("file '{}' not found", path.generic_string()));
}

void InMemoryFileSystem::add(std::filesystem::path const& path, std::vector<char>&& buffer, bool isRegularFile, bool exists)
{
    _files.emplace(path, FileInfo{exists, isRegularFile, std::move(buffer)});
}
