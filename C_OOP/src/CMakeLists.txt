aux_source_directory(./ SRC_PATH)

add_executable(C_OOP "")

target_sources(
    C_OOP
    PRIVATE
        ${SRC_PATH}   
)