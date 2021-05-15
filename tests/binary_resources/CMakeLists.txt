add_executable(binary_resources_test main.cpp)
add_test(NAME binary_resources_test COMMAND project_test)
target_link_libraries(binary_resources_test PRIVATE Catch2::Catch2WithMain)
rescom_compile(binary_resources_test ${CMAKE_CURRENT_SOURCE_DIR}/resources/files.rescom)
