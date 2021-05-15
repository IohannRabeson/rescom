project(RescomProjectTest)

add_executable(RescomProjectTest main.cpp)
rescom_compile(RescomProjectTest ${CMAKE_CURRENT_SOURCE_DIR}/resources/files.rescom)
add_test(NAME RescomProjectTest COMMAND RescomProjectTest)