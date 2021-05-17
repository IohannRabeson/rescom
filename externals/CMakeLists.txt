add_subdirectory(cxxopts)
add_subdirectory(fmt)
add_subdirectory(lexy)

if (RESCOM_TEST)
add_subdirectory(Catch2)
endif()