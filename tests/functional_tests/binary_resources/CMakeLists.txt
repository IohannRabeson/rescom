add_executable(binary_resources_test main.cpp ../../unit_tests/main.cpp)
rescom_compile(binary_resources_test ${CMAKE_CURRENT_SOURCE_DIR}/resources/files.rescom)
common_tests(binary_resources_test)