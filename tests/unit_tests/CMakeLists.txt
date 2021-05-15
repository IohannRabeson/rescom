add_executable(unit_tests main.cpp StringsTest.cpp)
target_link_libraries(unit_tests PRIVATE Catch2::Catch2WithMain)
target_include_directories(unit_tests PRIVATE ${PROJECT_SOURCE_DIR}/compiler)
add_test(NAME unit_tests COMMAND unit_tests)