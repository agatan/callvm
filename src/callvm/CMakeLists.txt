find_package(Boost COMPONENTS system REQUIRED)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif ()

file(GLOB_RECURSE CPPFILES *.cpp)
add_library(callvm-lib ${CPPFILES})
target_link_libraries(callvm-lib ${REQUIRED_LLVM_LIBS} ${Boost_LIBRARIES})
