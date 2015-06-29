cmake_minimum_required(VERSION 2.8)
project(callvm)

set(CMAKE_CXX_COMPILER clang++)

set(CALLVM_VERSION_MAJOR 0)
set(CALLVM_VERSION_MINOR 0)
set(CALLVM_VERSION_TEENY 0)
set(PACKAGE_VERSION "${CALLVM_VERSION_MAJOR}.${CALLVM_VERSION_MINOR}.${CALLVM_VERSION_TEENY}")

message("SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
message("BINARY_DIR: ${PROJECT_BINARY_DIR}")

add_definitions("-Wall -std=c++1z")

add_subdirectory(src)
