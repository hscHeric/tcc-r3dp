cmake_minimum_required(VERSION 3.15)
project(tcc-hsc VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native -Wall")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenMP REQUIRED) #OpenMP

include_directories(${PROJECT_SOURCE_DIR}/src)

add_library(common STATIC
    src/common/graph.cpp
)
target_link_libraries(common PUBLIC OpenMP::OpenMP_CXX)

add_subdirectory(examples)
