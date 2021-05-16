macro (common_tests TARGET_NAME)
    target_compile_options(${TARGET_NAME} PRIVATE
            $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
            -Werror -Wall -Wextra>
            $<$<CXX_COMPILER_ID:MSVC>:
            /WX /W4>
            )
    set_target_properties(${TARGET_NAME} PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS ON CXX_STANDARD_REQUIRED ON)
    add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})
    target_link_libraries(${TARGET_NAME} PRIVATE Catch2::Catch2WithMain)
endmacro()

add_subdirectory(only_text_resources)
add_subdirectory(empty_project_test)
add_subdirectory(binary_resources)
add_subdirectory(comments)
add_subdirectory(resource_directory)
add_subdirectory(non_recursive)