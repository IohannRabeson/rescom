#ifndef RESCOM_NAIVECPPGENERATOR_HPP
#define RESCOM_NAIVECPPGENERATOR_HPP
#include <ostream>
#include <vector>
#include <string>

#include "CodeGenerator.hpp"

struct Configuration;
struct Input;

class NaiveCppGenerator : public CodeGenerator
{
public:
    explicit NaiveCppGenerator(Configuration const& configuration);

private:
    void generate(std::ostream& output) override;

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

#endif //RESCOM_NAIVECPPGENERATOR_HPP
