add_executable(rescom main.cpp Configuration.cpp Configuration.hpp CodeGenerator.cpp CodeGenerator.hpp StringHelpers.hpp StringHelpers.cpp RescomFileParser.cpp RescomFileParser.hpp)
target_link_libraries(rescom PRIVATE cxxopts fmt lexy)
set_target_properties(rescom PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS ON CXX_STANDARD_REQUIRED ON)
warning_as_error(rescom)