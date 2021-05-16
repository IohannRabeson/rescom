add_executable(resource_directory_tests main.cpp ../../unit_tests/main.cpp)
rescom_compile(resource_directory_tests ${CMAKE_CURRENT_SOURCE_DIR}/resources/files.rescom RECURSIVE)
common_tests(resource_directory_tests)