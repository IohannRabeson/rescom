add_executable(empty_project_test main.cpp)
add_test(NAME empty_project_test COMMAND empty_project_test)
target_link_libraries(empty_project_test PRIVATE gtest gtest_main)
rescom_compile(empty_project_test ${CMAKE_CURRENT_SOURCE_DIR}/resources/files.rescom)
