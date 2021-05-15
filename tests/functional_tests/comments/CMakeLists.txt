add_executable(comments_tests main.cpp)
add_test(NAME comments_tests COMMAND project_test)
target_link_libraries(comments_tests PRIVATE Catch2::Catch2WithMain)
rescom_compile(comments_tests ${CMAKE_CURRENT_SOURCE_DIR}/resources/files.rescom)
