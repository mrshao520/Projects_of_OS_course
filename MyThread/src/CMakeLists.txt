aux_source_directory(./ SRC_PATH)

# find_package(spdlog REQUIRED)

# spdlog::spdlog
# spdlog::spdlog_header_only
# if(TARGET spdlog::spdlog)
#     message(STATUS "Find spdlog")
# else()
#     message(STATUS "Con't find the spdlog")
# endif()

add_executable(MyThread "")

target_sources(
    MyThread
    PRIVATE
        ${SRC_PATH}   
)

target_link_libraries(
    MyThread
    PRIVATE
        pthread
        # spdlog::spdlog   
)