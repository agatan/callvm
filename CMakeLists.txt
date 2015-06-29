cmake_minimum_required(VERSION 2.8)
project(callvm)

set(CMAKE_CXX_COMPILER clang++)

set(CALLVM_VERSION_MAJOR 0)
set(CALLVM_VERSION_MINOR 0)
set(CALLVM_VERSION_TEENY 1)
set(PACKAGE_VERSION "${CALLVM_VERSION_MAJOR}.${CALLVM_VERSION_MINOR}.${CALLVM_VERSION_TEENY}")

set(CALLVM_LLVM_CONFIG "llvm-config")
if (EXISTS "/usr/bin/${CALLVM_LLVM_CONFIG}")
    set(CALLVM_LLVM_CONFIG "/usr/bin/${CALLVM_LLVM_CONFIG}")
elseif (EXISTS "/usr/local/bin/${CALLVM_LLVM_CONFIG}")
    set(CALLVM_LLVM_CONFIG "/usr/local/bin/${CALLVM_LLVM_CONFIG}")
else ()
    message(FATAL_ERROR "'${CALLVM_LLVM_CONFIG}' is not found.")
endif()
message(STATUS "detected for llvm-config: ${CALLVM_LLVM_CONFIG}")

# LLVM configurations
execute_process(
    COMMAND
    ${CALLVM_LLVM_CONFIG} --version
    OUTPUT_VARIABLE
    LLVM_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
execute_process(
    COMMAND
    ${CALLVM_LLVM_CONFIG} --includedir
    OUTPUT_VARIABLE
    LLVM_INCLUDE_DIRS
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
execute_process(
    COMMAND
    ${CALLVM_LLVM_CONFIG} --libdir
    OUTPUT_VARIABLE
    LLVM_LIB_DIRS
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )

include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIB_DIRS})

message(STATUS "LLVM version: ${LLVM_VERSION}")
message(STATUS "LLVM includedir: ${LLVM_INCLUDE_DIRS}")
message(STATUS "LLVM libdir: ${LLVM_LIB_DIRS}")

add_definitions("-Wall -std=c++1z")

add_subdirectory(src)
