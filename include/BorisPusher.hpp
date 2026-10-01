#pragma once

#include <vector>

#include "FieldGrid.hpp"
#include "Particle.hpp"

// Advance one particle by one time step.
void borisPush(
    Particle& p,
    const Vector3& E,
    const Vector3& B,
    double dt
);

// Advance all particles in the same electromagnetic fields.
void borisPush(
    std::vector<Particle>& particles,
    const Vector3& E,
    const Vector3& B,
    double dt
);

// Advance particles using fields already evaluated at their positions.
void borisPush(
    std::vector<Particle>& particles,
    const std::vector<Vector3>& E_part,
    const std::vector<Vector3>& B_part,
    double dt
);

// Place a particle inside the periodic x domain of the grid.
void applyPeriodicBoundary(
    Particle& p,
    const FieldGrid& grid
);

// Interpolate grid fields, advance the particles and apply
// the periodic boundary condition in the x direction.
void borisPush(
    std::vector<Particle>& particles,
    const FieldGrid& grid,
    double dt
);