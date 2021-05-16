add_executable(empty_project_test main.cpp)
rescom_compile(empty_project_test ${CMAKE_CURRENT_SOURCE_DIR}/resources/files.rescom)
common_tests(empty_project_test)