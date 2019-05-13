find_package(Catch2 REQUIRED)

add_executable(run_test catch_config.cpp run_test.cpp)
target_link_libraries(run_test Catch2::Catch2)
target_link_libraries(run_test ArrayFire::afcpu)