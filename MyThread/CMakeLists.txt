cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(MyThread VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
# enable testing
# ---------------------------------------------------------------------------------------
include(CTest)
enable_testing()

add_subdirectory(src)