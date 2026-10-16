#pragma once

#include <vector>

#include "Grid.hpp"
#include "Particle.hpp"

// Particle quantities accumulated on grid nodes.
struct Moments {
    // Particle number per unit length.
    std::vector<double> density;

    // Density multiplied by mean velocity.
    std::vector<Vector3> flux;

    // Mean particle velocity.
    std::vector<Vector3> velocity;

    // Total kinetic energy per unit length.
    std::vector<double> energy;

    // Temperature in normalized units.
    std::vector<double> temperature;
};

// Deposit particle moments on the periodic grid.
Moments depositMoments(
    const std::vector<Particle>& particles,
    const Grid& grid
);