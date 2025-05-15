cmake_minimum_required(VERSION 3.10)
project(runner)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(DISABLE_TESTS "Disable tests generation" ON)

file(GLOB_RECURSE sources src/*.c)
list(FILTER sources EXCLUDE REGEX "\\.test\\.")
add_executable(runner ${sources} runner.c)
set_target_properties(runner PROPERTIES LINK_FLAGS -fsanitize=address)
set_target_properties(runner PROPERTIES COMPILE_FLAGS "-g -fsanitize=address")
include_directories(runner src)
file(COPY config DESTINATION ${CMAKE_BINARY_DIR})

if (NOT DISABLE_TESTS)
    find_library(CUNIT_LIBRARY NAMES
        cunit
        libcunit
        cunitlib
    )
    mark_as_advanced(CUNIT_LIBRARY)

    file(GLOB_RECURSE test_sources src/*.c)
    add_executable(runner-tests ${test_sources} tests.c)
    set_target_properties(runner-tests PROPERTIES LINK_FLAGS -fsanitize=address)
    set_target_properties(runner-tests PROPERTIES COMPILE_FLAGS "-g -fsanitize=address")
    include_directories(runner-tests src)
    target_link_libraries(runner-tests ${CUNIT_LIBRARY})
endif()
