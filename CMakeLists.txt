cmake_minimum_required(VERSION 2.8)
project(callvm)

set(CMAKE_CXX_COMPILER clang++)

set(CALLVM_VERSION_MAJOR 0)
set(CALLVM_VERSION_MINOR 0)
set(CALLVM_VERSION_TEENY 1)
set(PACKAGE_VERSION "${CALLVM_VERSION_MAJOR}.${CALLVM_VERSION_MINOR}.${CALLVM_VERSION_TEENY}")

execute_process(
    COMMAND
    which llvm-config
    OUTPUT_VARIABLE
    CALLVM_LLVM_CONFIG
    ERROR_VARIABLE
    CALLVM_LLVM_CONFIG_ERROR
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
if(CALLVM_LLVM_CONFIG_ERROR)
    message(FATAL_ERROR "'llvm-config' is not found: ${CALLVM_LLVM_CONFIG_ERROR}")
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
execute_process(
    COMMAND
    ${CALLVM_LLVM_CONFIG} --ldflags
    OUTPUT_VARIABLE
    LLVM_SYS_LIBS_STRING
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
string(REGEX MATCHALL "-l([^ ]+)" LLVM_SYS_LIBS ${LLVM_SYS_LIBS_STRING})

execute_process(
    COMMAND
    ${CALLVM_LLVM_CONFIG} --libs core
    OUTPUT_VARIABLE
    LLVM_LIBS_STRING
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
string(REGEX MATCHALL "-l([^ ]+)" LLVM_LIBS ${LLVM_LIBS_STRING})

set(REQUIRED_LLVM_LIBS)
foreach (l ${LLVM_LIBS} ${LLVM_SYS_LIBS})
    string(REGEX REPLACE "-l([^ ]+)" "\\1" lib_name ${l})
    list(APPEND REQUIRED_LLVM_LIBS ${lib_name})
endforeach (l)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    list(APPEND REQUIRED_LLVM_LIBS "pthread" "curses" "dl")
endif()

include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIB_DIRS})
add_definitions(-D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS)


message(STATUS "LLVM version: ${LLVM_VERSION}")
message(STATUS "LLVM includedir: ${LLVM_INCLUDE_DIRS}")
message(STATUS "LLVM libdir: ${LLVM_LIB_DIRS}")

add_definitions("-Wall -std=c++1z")

add_subdirectory(src)
