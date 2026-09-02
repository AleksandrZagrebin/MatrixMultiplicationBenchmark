cmake_minimum_required(VERSION 3.16)
project(MatrixBenchmark)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external)

add_library(matrix_lib INTERFACE)
target_include_directories(matrix_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/matrixlib)

add_executable(squarebench src/squarematrixbench.cpp)
target_link_libraries(squarebench PRIVATE matrix_lib)

add_executable(bench src/matrixbenchmark.cpp)
target_link_libraries(bench PRIVATE matrix_lib)

add_executable(plotter src/plot.cpp)
target_link_libraries(plotter PRIVATE matrix_lib)

target_compile_options(squarebench PRIVATE -O2)
target_compile_options(plotter PRIVATE -O2)
target_compile_options(bench PRIVATE -O2)
