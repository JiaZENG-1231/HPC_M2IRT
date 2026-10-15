cmake_minimum_required(VERSION 3.16)

project(
    HPC_PIC
    VERSION 0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Physics library shared by the programs and tests.
add_library(
    pic_core
    src/BorisPusher.cpp
    src/Grid.cpp
)

target_include_directories(
    pic_core
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Main program.
add_executable(
    pic_demo
    src/main.cpp
)

target_link_libraries(
    pic_demo
    PRIVATE
    pic_core
)

# Vector tests.
add_executable(
    test_vector3
    tests/test_vector3.cpp
)

target_link_libraries(
    test_vector3
    PRIVATE
    pic_core
)

# Boris pusher tests.
add_executable(
    test_boris
    tests/test_boris.cpp
)

target_link_libraries(
    test_boris
    PRIVATE
    pic_core
)

# Single-particle trajectory in a uniform magnetic field.
add_executable(
    particle_trajectory
    src/particle_trajectory.cpp
)

target_link_libraries(
    particle_trajectory
    PRIVATE
    pic_core
)

# Periodic grid and shape-weight tests.
add_executable(
    test_grid
    tests/test_grid.cpp
)

target_link_libraries(
    test_grid
    PRIVATE
    pic_core
)

enable_testing()

add_test(
    NAME vector3_tests
    COMMAND test_vector3
)

add_test(
    NAME boris_tests
    COMMAND test_boris
)

add_test(
    NAME grid_tests
    COMMAND test_grid
)