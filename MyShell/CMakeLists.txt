cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(MyShell VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MAIN_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# ---------------------------------------------------------------------------------------
# set build type
# ---------------------------------------------------------------------------------------
message(STATUS "CMAKE_BUILD_TYPE : ${CMAKE_BUILD_TYPE}")

if(CMAKE_BUILD_TYPE)
    string(TOLOWER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
    if(${BUILD_TYPE} STREQUAL "debug")
        set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} -Wno-deprecated-declarations -Wall -O0")
    elseif(${BUILD_TYPE} STREQUAL "release")
        set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_RELEASE} -Wno-deprecated-declarations -Wall -O3")
    else()
        set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} -Wno-deprecated-declarations -Wall -O0")
        message("CMAKE_BUILD_TYPE set to unknown string,so set Default CMAKE_BUILD_TYPE:${CMAKE_BUILD_TYPE}")
    endif()
else()
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} -Wno-deprecated-declarations -Wall -O0")
    message("Set Default CMAKE_BUILD_TYPE:${CMAKE_BUILD_TYPE}")
endif()


# add -fPIC
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
# add -fpermissive
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")



# ---------------------------------------------------------------------------------------
# install program
# ---------------------------------------------------------------------------------------
message(STATUS "Project will be installed to ${CMAKE_INSTALL_PREFIX}")

# set(CMAKE_INSTALL_PREFIX "/home/test/MyShell/local")

include(GNUInstallDirs)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

# Offer the user the choice of overriding the installation directories
# 目标代码库(即静态库和动态库)所在的子目录
set(INSTALL_LIBDIR ${CMAKE_INSTALL_LIBDIR} CACHE PATH "Installation directory for libraries")
# 用户可执行文件所在的子目录
set(INSTALL_BINDIR ${CMAKE_INSTALL_BINDIR} CACHE PATH "Installation directory for executables")
# 头文件
set(INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR} CACHE PATH "Installation directory for header files")


if(WIN32 AND NOT CYGWIN)
    set(DEF_INSTALL_CMAKEDIR CMake)
else()
    set(DEF_INSTALL_CMAKEDIR share/cmake/${PROJECT_NAME})
endif()
set(INSTALL_CMAKEDIR ${DEF_INSTALL_CMAKEDIR} CACHE PATH "Installation directory for CMake files")

message(STATUS "Install cmake directory : ${INSTALL_CMAKEDIR}")

# Report to user
foreach(p LIB BIN INCLUDE CMAKE)
    file(TO_NATIVE_PATH ${CMAKE_INSTALL_PREFIX}/${INSTALL_${p}DIR} _path)
    message(STATUS "Installing ${p} components to ${_path}")
    unset(_path)
endforeach()




# ---------------------------------------------------------------------------------------
# enable testing
# ---------------------------------------------------------------------------------------
include(CTest)
enable_testing()

add_subdirectory(src)
add_subdirectory(tests)