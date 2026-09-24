#pragma once

#include <vector>

#include "FieldGrid.hpp"
#include "Particle.hpp"

// Particle moments accumulated on grid points.
struct GridMoments {
    // Sum of macroparticle weights on each grid point.
    std::vector<double> density;

    // Weighted mean particle velocity on each grid point.
    std::vector<Vector3> velocity;
};

// Deposit particle charge using an order-1 B-spline.
std::vector<double> depositCharge(
    const std::vector<Particle>& particles,
    const FieldGrid& grid
);

// Compute particle density and mean velocity on the grid.
GridMoments computeMoments(
    const std::vector<Particle>& particles,
    const FieldGrid& grid
);