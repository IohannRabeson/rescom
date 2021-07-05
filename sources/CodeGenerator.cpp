#include "CodeGenerator.hpp"
#include "NaiveCppGenerator.hpp"
#include "StringHelpers.hpp"

#include <map>
#include <cassert>
#include <stdexcept>

std::map<std::string, std::function<CodeGeneratorPointer(Configuration const&)>> _factory;

decltype(_factory)::key_type _defaultCodeGeneratorKey;

void registerCodeGenerator(std::string const& key, std::function<CodeGeneratorPointer(Configuration const&)>&& creator, bool setDefault)
{
    // Check for duplicate
    assert( _factory.find(key) == _factory.end() );

    _factory[key] = std::move(creator);

    if (setDefault)
        _defaultCodeGeneratorKey = key;
}

CodeGeneratorPointer instanciateCodeGenerator(std::string const& key, Configuration const& configuration)
{
    if (auto it = _factory.find(key); it != _factory.end())
        return it->second(configuration);

    throw std::runtime_error(format("invalid code generator key: '{}'", key));
}

CodeGeneratorPointer instanciateDefaultCodeGenerator(Configuration const& configuration)
{
    return instanciateCodeGenerator(_defaultCodeGeneratorKey, configuration);
}