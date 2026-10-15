#pragma once

#include <vector>

#include "Grid.hpp"
#include "Particle.hpp"

// Particle quantities accumulated on grid nodes.
struct Moments {
    std::vector<double> density;
    std::vector<Vector3> flux;
    std::vector<Vector3> velocity;
};

// Deposit particle density, flux and mean velocity.
Moments depositMoments(
    const std::vector<Particle>& particles,
    const Grid& grid
);