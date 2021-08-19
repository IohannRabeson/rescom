add_subdirectory_if_target_not_exists(cxxopts)

add_library(PicoSHA2 INTERFACE)
target_include_directories(PicoSHA2 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/PicoSHA2)

if (RESCOM_TEST)
    add_subdirectory_if_target_not_exists(Catch2)
endif()