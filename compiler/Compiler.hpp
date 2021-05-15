#ifndef RESCOM_COMPILER_HPP
#define RESCOM_COMPILER_HPP
#include <ostream>
#include <vector>
#include <string>

class Configuration;
class Input;

enum CompilationResult
{
    Ok,
    Error,
};

class Compiler
{
public:
    explicit Compiler(Configuration const& configuration);

    CompilationResult compile(std::ostream& output);
private:
    std::string tab(unsigned int count = 1) const;

    void writeFileHeader(std::ostream& output);
    void writeFileFooter(std::ostream& output);
    void writeResource(Input const& input, std::vector<char> const& buffer, std::ostream& output);
    void writeAccessFunction(std::ostream& output);
    CompilationResult writeResources(std::ostream& output);
private:
    Configuration const& _configuration;
    std::string const _tabulation;
};

#endif //RESCOM_COMPILER_HPP
