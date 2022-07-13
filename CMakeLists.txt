cmake_minimum_required(VERSION 3.8)

project(arg-parse
        DESCRIPTION "Command line argument parsing for C++ 17 and above"
        VERSION 0.1.0)

add_library(arg-parse INTERFACE
        # Headers
        include/arg-parse/arg-parse.h
        include/arg-parse/parser.h
        )
target_compile_features(arg-parse INTERFACE cxx_std_17)
target_include_directories(arg-parse INTERFACE include/)

option(ARG_PARSE_BUILD_TESTS "Build tests for arg-parse" OFF)
if (ARG_PARSE_BUILD_TESTS)
    # Include googletest - Taken from https://google.github.io/googletest/quickstart-cmake.html#set-up-a-project
    include(FetchContent)
    FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/4219e7254cb8c473f57f6065bd13d1520d7b708f.zip
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()
    include(GoogleTest)
    add_subdirectory(tests)
endif()
