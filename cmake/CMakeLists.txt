function(rescom_compile TARGET_NAME RESCOM_FILE)
    target_sources(${TARGET_NAME} PRIVATE rescom.hpp)

    add_custom_command(
            COMMAND rescom -i ${RESCOM_FILE} -o rescom.hpp
            DEPENDS ${RESCOM_FILE} rescom
            OUTPUT rescom.hpp
            COMMENT "Compiling resources..."
    )

    target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
    set_target_properties(${TARGET_NAME} PROPERTIES CXX_STANDARD 17)
endfunction()