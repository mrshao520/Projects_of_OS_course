aux_source_directory(./ SRC_PATH)

find_package(spdlog REQUIRED)

# spdlog::spdlog
# spdlog::spdlog_header_only
if(TARGET spdlog::spdlog)  
    message(STATUS "Find spdlog")
else()
    message(STATUS "Con't find the spdlog")
endif()

add_executable(MyShell "")

target_sources(
    MyShell
    PRIVATE
        ${SRC_PATH}   
)

target_link_libraries(
    MyShell
    PRIVATE
        pthread
        spdlog::spdlog   
)


# Prepare rpath
file(RELATIVE_PATH _rel ${CMAKE_INSTALL_PREFIX}/${INSTALL_BINDIR} ${CMAKE_INSTALL_PREFIX})
if(APPLE)
    set(_rpath "@loader_path/${_rel}")
else()
    set(_rpath "\$ORIGIN/${_rel}")
endif()
file(TO_NATIVE_PATH "${_rpath}/${INSTALL_LIBDIR}" message_RPATH)
message(STATUS "message path : ${message_RPATH}")

set_target_properties(
    MyShell
    PROPERTIES
        MACOSX_PRATH ON   # 告诉CMake生成适当的RPATH，以便能够在构建树中运行可执行文件。
        SKIP_BUILD_RPATH OFF
        BUILD_WITH_INSTALL_RPATH OFF
        INSTALL_RPATH "${message_RPATH}"
        INSTALL_RPATH_USE_LINK_PATH ON   
)

# Install and export targets
install(
    TARGETS
        MyShell
    ARCHIVE    # 静态库将安装到ARCHIVE DESTINATION，将其设置为${INSTALL_LIBDIR}:
        DESTINATION ${INSTALL_LIBDIR}
        COMPONENT lib
    RUNTIME    # 可执行文件将安装在RUNTIME DESTINATION，将其设置为${INSTALL_BINDIR}
        DESTINATION ${INSTALL_BINDIR}
        COMPONENT bin 
    LIBRARY    # 动态库安装到LIBRARY_DESTINATION，将其设置为${INSTALL_LIBDIR}
        DESTINATION ${INSTALL_LIBDIR}
        COMPONENT lib
    PUBLIC_HEADER     # 头文件安装到PUBLIC_HEADER DESTINATION，将其设置为${INSTALL_INCLUDIR}/message
        DESTINATION ${INSTALL_INCLUDEDIR}/message
        COMPONENT dev
)