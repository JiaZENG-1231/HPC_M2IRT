cmake_minimum_required(VERSION 3.16)

project(
    HPC_M2IRT
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Core Boris pusher and particle-mesh operations.
add_library(
    boris_core
    src/BorisPusher.cpp
    src/FieldGrid.cpp
    src/ParticleMesh.cpp
)

target_include_directories(
    boris_core
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Basic example.
add_executable(
    boris
    src/main.cpp
)

target_link_libraries(
    boris
    PRIVATE
    boris_core
)

# Single-particle magnetic-field simulation.
add_executable(
    simulation
    src/simulation.cpp
)

target_link_libraries(
    simulation
    PRIVATE
    boris_core
)

# N-particle magnetic-field simulation.
add_executable(
    multi_particle_simulation
    src/multi_particle_simulation.cpp
)

target_link_libraries(
    multi_particle_simulation
    PRIVATE
    boris_core
)

# Unit and physical validation tests.
add_executable(
    test_boris
    tests/test_boris.cpp
)

target_link_libraries(
    test_boris
    PRIVATE
    boris_core
)

enable_testing()

add_test(
    NAME boris_tests
    COMMAND test_boris
)