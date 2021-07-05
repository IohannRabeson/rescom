#ifndef RESCOM_CODEGENERATOR_HPP
#define RESCOM_CODEGENERATOR_HPP
#include <ostream>
#include <memory>
#include <string>
#include <functional>

struct Configuration;

class CodeGenerator
{
public:
    virtual ~CodeGenerator() = default;

    virtual void generate(std::ostream& output) = 0;
};

using CodeGeneratorPointer = std::unique_ptr<CodeGenerator>;

void registerCodeGenerator(std::string const& key, std::function<CodeGeneratorPointer(Configuration const&)>&& creator, bool setDefault = false);
CodeGeneratorPointer instanciateCodeGenerator(std::string const& key, Configuration const& configuration);
CodeGeneratorPointer instanciateDefaultCodeGenerator(Configuration const& configuration);

#endif //RESCOM_CODEGENERATOR_HPP
