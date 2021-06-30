add_executable(duplicate_tests main.cpp)
rescom_compile(duplicate_tests ${CMAKE_CURRENT_SOURCE_DIR}/resources/files.rescom)
target_compile_definitions(duplicate_tests PRIVATE RESOURCES_DIRECTORY="${CMAKE_CURRENT_SOURCE_DIR}/resources")
common_tests(duplicate_tests)