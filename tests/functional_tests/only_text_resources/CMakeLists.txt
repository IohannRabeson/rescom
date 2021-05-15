project(project_test)

add_executable(project_test main.cpp)
add_test(NAME project_test COMMAND project_test)
target_link_libraries(project_test PRIVATE Catch2::Catch2WithMain)
rescom_compile(project_test ${CMAKE_CURRENT_SOURCE_DIR}/resources/files.rescom)
