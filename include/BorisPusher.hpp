#pragma once

#include "Particle.hpp"

// Advance one particle through constant local fields.
void borisPush(
    Particle& p,
    const Vector3& E,
    const Vector3& B,
    double dt
);