#pragma once

#include <vector>

#include "Grid.hpp"
#include "Particle.hpp"

// Apply the periodic x boundary to one particle.
void applyPeriodicBoundary(
    Particle& p,
    const Grid& grid
);

// Apply the periodic x boundary to all particles.
void applyPeriodicBoundary(
    std::vector<Particle>& particles,
    const Grid& grid
);