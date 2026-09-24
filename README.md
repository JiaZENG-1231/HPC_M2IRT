# Boris Particle Pusher

This project implements a Boris particle pusher in C++ for advancing charged
particles in electric and magnetic fields.

The implementation starts with a single-particle pusher and extends it to
multiple particles, field interpolation, and particle-to-grid deposition.

## Features

- Three-dimensional vector operations
- Single-particle Boris pusher
- Multiple-particle Boris pusher
- Different local fields for different particles
- Linear field interpolation from a one-dimensional grid
- Order-1 B-spline particle-to-grid deposition
- Macroparticle statistical weights
- Grid density and mean-velocity moments
- Physical validation tests
- CSV output and Python visualization

The spatial mesh is currently one-dimensional, while particle positions,
velocities, and electromagnetic fields remain three-dimensional.

## Boris algorithm

For one time step, the particle velocity is updated using:

1. Half of the electric-field acceleration
2. Rotation in the magnetic field
3. The second half of the electric-field acceleration
4. Position update using the new velocity

This splitting allows the algorithm to conserve particle speed accurately in
a purely magnetic field.

## Particle-mesh weighting

Field interpolation and particle deposition use an order-1 B-spline, also
known as linear Cloud-In-Cell weighting.

For a particle between grid points `i` and `i + 1`, the weights are:

```text
w_left  = 1 - (x_p - x_i) / dx
w_right =     (x_p - x_i) / dx
```

The same linear weights are used both for field interpolation and for
accumulating particle quantities on the grid.

## Project structure

```text
include/
    BorisPusher.hpp
    FieldGrid.hpp
    Particle.hpp
    ParticleMesh.hpp
    Vector3.hpp

src/
    BorisPusher.cpp
    FieldGrid.cpp
    ParticleMesh.cpp
    main.cpp
    simulation.cpp
    multi_particle_simulation.cpp

tests/
    test_boris.cpp

scripts/
    plot_results.py
    plot_multi_particles.py

results/
    boris_validation.png
    multi_particle_validation.png
```

## Requirements

The C++ code requires:

- A C++17 compiler
- CMake 3.16 or later

The plotting scripts require:

- Python
- NumPy
- Matplotlib

The Python packages can be installed with:

```bash
conda install numpy matplotlib
```

## Build

From the project root directory, run:

```bash
cmake -S . -B build
cmake --build build
```

The compiled programs will be placed inside the `build` directory.

## Run the tests

Run all tests through CMake:

```bash
ctest --test-dir build --output-on-failure
```

The test executable can also be run directly:

```bash
./build/test_boris
```

The tests cover:

- Free particle motion
- Acceleration in a constant electric field
- Speed conservation in a constant magnetic field
- Multiple-particle advancement
- Different local fields for different particles
- Linear field interpolation
- Grid-to-particle coupling
- Charge deposition and charge conservation
- Grid density and mean velocity

## Single-particle simulation

Create the output directory if necessary:

```bash
mkdir -p results
```

Run the simulation from the project root directory:

```bash
./build/simulation
```

The simulation saves particle positions, velocities, speed, electric field,
and magnetic field in:

```text
results/particle_motion.csv
```

Plot the results with:

```bash
python scripts/plot_results.py
```

The figure is saved as:

```text
results/boris_validation.png
```

For this test, a positively charged particle moves in a constant magnetic
field with:

```text
q = 1
m = 1
|v| = 1
B_z = 1
```

The theoretical Larmor radius is:

```text
r_L = m |v| / (|q| B) = 1
```

The expected result is circular motion in the x-y plane with constant speed.

## Multiple-particle simulation

Run the multiple-particle simulation with:

```bash
./build/multi_particle_simulation
```

The results are saved in:

```text
results/multi_particle_motion.csv
```

Plot the results with:

```bash
python scripts/plot_multi_particles.py
```

The figure is saved as:

```text
results/multi_particle_validation.png
```

The particles have different initial speeds and charge signs. The resulting
plots demonstrate:

- Larmor radius proportional to particle speed
- Opposite rotation directions for positive and negative charges
- Speed conservation for every particle

## Validation results

In a purely magnetic field, the Lorentz force changes the direction of the
particle velocity but does not change its magnitude. Therefore, the kinetic
energy should remain constant.

The simulation reproduces:

- Circular particle trajectories
- The expected Larmor radii
- Opposite rotation for opposite charge signs
- Constant particle speeds

These results provide physical validation of both the single-particle and
multiple-particle Boris pushers.

## Current scope

The project includes particle pushing, mesh-to-particle interpolation, and
particle-to-mesh moment deposition.

The current implementation uses a one-dimensional uniform spatial grid and
three-dimensional particle and field vectors. It does not include a
self-consistent Maxwell field solver.