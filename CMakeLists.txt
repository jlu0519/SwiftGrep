cmake_minimum_required(VERSION 3.16)

project(SwiftGrep)


# SwiftGrep core library
add_library(swiftgrep_core
    src/argument_parser.cpp
    src/search.cpp
)

target_compile_features(
    swiftgrep_core
    PUBLIC
    cxx_std_20
)

target_include_directories(
    swiftgrep_core
    PUBLIC
    src
)


# Main SwiftGrep executable
add_executable(swiftgrep
    src/main.cpp
)

target_link_libraries(
    swiftgrep
    PRIVATE
    swiftgrep_core
)


# GoogleTest dependency
include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)

FetchContent_MakeAvailable(googletest)


# Enable testing
enable_testing()


# Unit test executable
add_executable(swiftgrep_tests
    tests/argument_parser_test.cpp
)

target_link_libraries(
    swiftgrep_tests
    PRIVATE
    swiftgrep_core
    GTest::gtest_main
)


# GoogleTest discovery
include(GoogleTest)

gtest_discover_tests(swiftgrep_tests)
