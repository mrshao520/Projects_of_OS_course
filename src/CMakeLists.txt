aux_source_directory(./ SRC_PATH)

find_package(spdlog REQUIRED)

# spdlog::spdlog
# spdlog::spdlog_header_only
if(TARGET spdlog::spdlog_header_only)
    message(STATUS "Find spdlog")
else()
    message(STATUS "Con't find the spdlog")
endif()

add_executable(MyGrep "")

target_sources(
    MyGrep 
    PRIVATE
        ${SRC_PATH}
)

target_link_libraries(
    MyGrep
    PRIVATE
        pthread
        spdlog::spdlog_header_only
)