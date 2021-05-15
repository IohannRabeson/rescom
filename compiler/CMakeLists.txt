add_executable(rescom main.cpp Configuration.cpp Configuration.hpp Compiler.cpp Compiler.hpp Strings.hpp)
target_link_libraries(rescom PRIVATE cxxopts)
set_target_properties(rescom PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON CXX_STANDARD_REQUIRED ON)
