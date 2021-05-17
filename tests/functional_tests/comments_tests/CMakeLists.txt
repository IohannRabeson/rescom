add_executable(comments_tests main.cpp)
rescom_compile(comments_tests ${CMAKE_CURRENT_SOURCE_DIR}/resources/files.rescom)
common_tests(comments_tests)