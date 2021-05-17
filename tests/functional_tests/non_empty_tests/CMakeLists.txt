add_executable(non_empty_tests main.cpp)
rescom_compile(non_empty_tests ${CMAKE_CURRENT_SOURCE_DIR}/resources/files.rescom)
target_compile_definitions(non_empty_tests PRIVATE RESOURCES_DIRECTORY="${CMAKE_CURRENT_SOURCE_DIR}/resources")
common_tests(non_empty_tests)