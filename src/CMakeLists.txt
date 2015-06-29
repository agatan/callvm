add_subdirectory(callvm)
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}")

add_executable(callvm
    main.cpp
)

target_link_libraries(callvm callvm-lib)
