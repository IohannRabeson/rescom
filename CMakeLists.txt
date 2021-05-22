# Options
# RESCOM_DEV_MODE ON/OFF If set to OFF rescom always build in Release mode
# RESCOM_TEST ON/OFF If set to ON tests build

cmake_minimum_required(VERSION 3.9)

project(rescom VERSION 0.0.0)

if (RESCOM_TEST)
    enable_testing()
endif()

add_subdirectory(cmake)
add_subdirectory(externals)
add_subdirectory(sources)
if (RESCOM_TEST)
    add_subdirectory(tests)
endif()