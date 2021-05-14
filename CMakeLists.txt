cmake_minimum_required(VERSION 3.9)

project(rescom)

find_package(RapidJSON)

add_subdirectory(externals)
add_executable(rescom main.cpp)
target_link_libraries(rescom PRIVATE cxxopts rapidjson)
set_target_properties(rescom PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS ON CXX_STANDARD_REQUIRED ON)
