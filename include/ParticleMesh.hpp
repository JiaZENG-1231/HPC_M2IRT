#pragma once

#include <vector>

#include "FieldGrid.hpp"
#include "Particle.hpp"

// Particle moments accumulated on the grid.
struct GridMoments {
    // Sum of macroparticle statistical weights at each grid point.
    std::vector<double> density;

    // Particle flux: sum of weight times velocity at each grid point.
    std::vector<Vector3> flux;

    // Bulk velocity obtained from flux divided by density.
    std::vector<Vector3> velocity;
};

// Deposit macroparticle charge using linear weighting.
std::vector<double> depositCharge(
    const std::vector<Particle>& particles,
    const FieldGrid& grid
);

// Deposit particle density and flux, then calculate bulk velocity.
GridMoments computeMoments(
    const std::vector<Particle>& particles,
    const FieldGrid& grid
);