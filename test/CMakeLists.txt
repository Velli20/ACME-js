


if ( NOT CMAKE_BUILD_TYPE MATCHES Debug )
    message(ERROR "CMAKE_BUILD_TYPE NOT EQUALS Debug")
endif()

# Build base library.

add_library(libacmejs
    STATIC
        ${PROJECT_SOURCE_DIR}/src/parse/parser.cpp
        ${PROJECT_SOURCE_DIR}/src/render/render.cpp
        ${PROJECT_SOURCE_DIR}/src/emit/emit.cpp
)

compiler_options(libacmejs)
precompiled_header(libacmejs)
std_polyfill_test(libacmejs)

target_include_directories(libacmejs
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/external/json/single_include
        ${PROJECT_SOURCE_DIR}/external/
)

if ( CMAKE_CXX_COMPILER_ID MATCHES ".*Clang" )
    option(ENABLE_BUILD_WITH_TIME_TRACE "Enable -ftime-trace to generate time tracing .json files on clang" OFF)
    if ( ENABLE_BUILD_WITH_TIME_TRACE )
        target_compile_definitions(libacmejs PRIVATE -ftime-trace)
    endif()
endif()

function(AddUnitTest)
    set(options        "")
    set(oneValueArgs   SOURCE_FILE)
    set(multiValueArgs HEADERS)

    cmake_parse_arguments(TEST "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    get_filename_component(TEST_NAME ${TEST_SOURCE_FILE} NAME_WE)

    message(INFO " Adding unit test ${TEST_SOURCE_FILE}")

    add_executable(${TEST_NAME} ${TEST_SOURCE_FILE} ${TEST_HEADERS})

    compiler_options(${TEST_NAME})

    target_precompile_headers(${TEST_NAME}
        REUSE_FROM
            libacmejs
    )

    add_test(
        NAME
            ${TEST_NAME}
        WORKING_DIRECTORY
            "${PROJECT_BINARY_DIR}/${location}"
        COMMAND
            $<TARGET_FILE:${TEST_NAME}>
    )

    target_include_directories(${TEST_NAME}
        PUBLIC
            ${PROJECT_SOURCE_DIR}/external/tts/include
    )

    target_link_libraries(${TEST_NAME}
        PRIVATE
            libacmejs
    )

endfunction(AddUnitTest)

file(GLOB UNIT_TESTS RELATIVE ${CMAKE_CURRENT_LIST_DIR} *test.cc)
list(SORT UNIT_TESTS)

foreach ( FILENAME ${UNIT_TESTS} )
    AddUnitTest(SOURCE_FILE ${FILENAME})
endforeach()

