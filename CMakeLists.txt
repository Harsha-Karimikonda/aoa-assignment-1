cmake_minimum_required(VERSION 3.12)
project(AOA_Assignment_1 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(COMMON_SOURCES
    src/graph.cpp
    src/graph_operations.cpp
    src/graph_simulator.cpp
    src/realgraph_make.cpp
)

add_executable(simulated_test
    ${COMMON_SOURCES}
    src/simulated_test.cpp
)

add_executable(run_realgraph_make
    ${COMMON_SOURCES}
    src/run_realgraph_make.cpp
)

if (WIN32)
    target_link_libraries(simulated_test psapi)
    target_link_libraries(run_realgraph_make psapi)
endif()
