cmake_minimum_required(VERSION 3.15)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

if ( BUILD_VERBOSE )
    set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "ON" FORCE)
endif()

if ( PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR )
    message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.\n")
endif()

option(CONFIG_USE_SANITZERS "Use sanitizers" ON)

set(CMAKE_CONFIGURATION_TYPES
    MinSizeRel
    Release
    Debug
)

if ( NOT CMAKE_BUILD_TYPE )
    set(CMAKE_BUILD_TYPE Debug)
endif()

if ( CMAKE_BUILD_TYPE MATCHES "Debug" )
    set(_enable_assertions TRUE)
else ()
    set(_enable_assertions FALSE)
endif()

option(ACME_JS_ENABLE_ASSERTIONS "Enable assertions" ${_enable_assertions})

message(STATUS "Build type ${CMAKE_BUILD_TYPE}")

project(acme
    VERSION
        0.0.1
    DESCRIPTION
        "JS"
    LANGUAGES
        C CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set CMake module path.

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# Modules to include.

include(CMakeParseArguments)
include(CheckCXXSourceCompiles)
include(compiler_options)
include(precompiled_header)
include(std_polyfill)

# include(FetchContent)
# FetchContent_Declare(
#     nlohmann-json URL https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz
# )
# FetchContent_MakeAvailable(nlohmann-json)


enable_testing()

# Sub-directories.

add_subdirectory(src)
add_subdirectory(test)

