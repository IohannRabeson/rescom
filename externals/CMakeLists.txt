add_subdirectory(cxxopts)
add_subdirectory(fmt)

if (RESCOM_TEST)
add_subdirectory(Catch2)
endif()