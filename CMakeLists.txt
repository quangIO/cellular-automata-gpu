cmake_minimum_required(VERSION 3.14)
project(cellular_automata)
FIND_PACKAGE(ArrayFire REQUIRED)


set(CMAKE_CXX_STANDARD 17)
include_directories(include)

add_library(simulation_lib SHARED simulation/simulation.cpp rules.cpp)

add_executable(cellular_automata main.cpp)

target_link_libraries(simulation_lib ArrayFire::af)
target_link_libraries(cellular_automata simulation_lib)

