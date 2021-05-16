add_executable(non_recursive_tests main.cpp ../../unit_tests/main.cpp)
rescom_compile(non_recursive_tests ${CMAKE_CURRENT_SOURCE_DIR}/resources/files.rescom)
common_tests(non_recursive_tests)