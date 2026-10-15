#pragma once

#include <vector>

#include "Particle.hpp"

// Advance one particle through constant local fields.
void borisPush(
    Particle& p,
    const Vector3& E,
    const Vector3& B,
    double dt
);

// Advance several particles through the same fields.
void borisPush(
    std::vector<Particle>& particles,
    const Vector3& E,
    const Vector3& B,
    double dt
);