set(RESCOM_SOURCES
    ${PROJECT_SOURCE_DIR}/sources/StringHelpers.cpp
    ${PROJECT_SOURCE_DIR}/sources/FileSystem.cpp
    ${PROJECT_SOURCE_DIR}/sources/ConfigurationParser.cpp
)
add_executable(unit_tests ${RESCOM_SOURCES} main.cpp StringsTest.cpp ConfigurationTests.cpp)
target_link_libraries(unit_tests PRIVATE Catch2::Catch2WithMain)
target_include_directories(unit_tests PRIVATE ${PROJECT_SOURCE_DIR}/sources)
set_target_properties(unit_tests PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS ON CXX_STANDARD_REQUIRED ON)
add_test(NAME unit_tests COMMAND unit_tests)