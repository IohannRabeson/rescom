#ifndef RESCOM_FILESYSTEM_HPP
#define RESCOM_FILESYSTEM_HPP
#include <filesystem>
#include <map>
#include <vector>

/// Abstract the filesystem for unit testing.
class FileSystem
{
public:
    virtual ~FileSystem() = default;
    virtual std::uint64_t fileSize(std::filesystem::path const& path) const = 0;
    virtual bool exists(std::filesystem::path const& path) const = 0;
    virtual bool isRegularFile(std::filesystem::path const& path) const = 0;
    virtual void getContent(std::filesystem::path const& path, std::vector<char>& buffer) const = 0;
};

/// Implementation using the local file system as data source.
class LocalFileSystem : public FileSystem
{
public:
    std::uint64_t fileSize(std::filesystem::path const& path) const override;
    bool exists(std::filesystem::path const& path) const override;
    bool isRegularFile(std::filesystem::path const& path) const override;
    void getContent(std::filesystem::path const& path, std::vector<char>& buffer) const override;
};

/// Implementation storing data in memory.
class InMemoryFileSystem : public FileSystem
{
    struct FileInfo
    {
        bool exists{false};
        bool isRegularFile{false};
        std::vector<char> buffer;
    };

    std::map<std::filesystem::path, FileInfo> _files;
public:
    /// Add a fake file to the filesystem.
    void add(std::filesystem::path const& path, std::vector<char>&& buffer, bool isRegularFile = true, bool exists = true);

    std::uint64_t fileSize(std::filesystem::path const& path) const override;
    bool exists(std::filesystem::path const& path) const override;
    bool isRegularFile(std::filesystem::path const& path) const override;
    void getContent(std::filesystem::path const& path, std::vector<char>& buffer) const override;
};

#endif //RESCOM_FILESYSTEM_HPP
