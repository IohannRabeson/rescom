add_executable(unit_tests
        main.cpp
        StringsTest.cpp
        ${PROJECT_SOURCE_DIR}/sources/StringHelpers.cpp
        ${PROJECT_SOURCE_DIR}/sources/RescomFileParser.cpp
        RescomFileParserTests.cpp)
target_link_libraries(unit_tests PRIVATE Catch2::Catch2WithMain lexy)
target_include_directories(unit_tests PRIVATE ${PROJECT_SOURCE_DIR}/sources)
set_target_properties(unit_tests PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS ON CXX_STANDARD_REQUIRED ON)
add_test(NAME unit_tests COMMAND unit_tests)