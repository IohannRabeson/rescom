cmake_minimum_required(VERSION 3.9)

project(rescom)

if (RESCOM_TEST)
    enable_testing()
endif()

add_subdirectory(cmake)
add_subdirectory(externals)
add_subdirectory(sources)
if (RESCOM_TEST)
    add_subdirectory(tests)
endif()