add_subdirectory_if_target_not_exists(cxxopts)

if (RESCOM_TEST)
    add_subdirectory_if_target_not_exists(Catch2)
endif()