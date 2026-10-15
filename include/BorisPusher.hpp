#pragma once

#include <vector>

#include "Fields.hpp"
#include "Grid.hpp"
#include "Particle.hpp"

// Advance one particle in local fields.
void borisPush(
    Particle& p,
    const Vector3& E,
    const Vector3& B,
    double dt
);

// Advance particles in uniform fields.
void borisPush(
    std::vector<Particle>& particles,
    const Vector3& E,
    const Vector3& B,
    double dt
);

// Gather local fields and advance particles.
void borisPush(
    std::vector<Particle>& particles,
    const VectorField& E,
    const VectorField& B,
    const Grid& grid,
    double dt
);