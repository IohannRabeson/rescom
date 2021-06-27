#ifndef RESCOM_CODEGENERATOR_HPP
#define RESCOM_CODEGENERATOR_HPP
#include <ostream>
#include <vector>
#include <string>

struct Configuration;
struct Input;

enum CompilationResult
{
    Ok,
    Error,
};

class CodeGenerator
{
public:
    explicit CodeGenerator(Configuration const& configuration);

    void generate(std::ostream& output);
private:
    std::string tab(unsigned int count = 1) const;

    void writeFileHeader(std::ostream& output) const;
    void writeFileFooter(std::ostream& output) const;
    void writeResource(Input const& input, unsigned int inputPosition, std::vector<char> const& buffer, std::ostream& output) const;
    void writeAccessFunction(std::ostream& output) const;
    void writeResources(std::ostream& output) const;
private:
    Configuration const& _configuration;
    std::string const _tabulation;
    std::string const _headerProtectionMacroName;
};

#endif //RESCOM_CODEGENERATOR_HPP
