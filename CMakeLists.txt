cmake_minimum_required(VERSION 3.16)

project(SwiftGrep)

add_executable(swiftgrep
    src/main.cpp
    src/argument_parser.cpp
    src/search.cpp
)

target_compile_features(swiftgrep PRIVATE cxx_std_20)
