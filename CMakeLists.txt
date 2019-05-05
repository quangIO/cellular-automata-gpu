cmake_minimum_required(VERSION 3.14)
project(cellular_automata)
FIND_PACKAGE(ArrayFire REQUIRED)


set(CMAKE_CXX_STANDARD 17)

add_executable(cellular_automata main.cpp)
target_link_libraries(cellular_automata ArrayFire::af)