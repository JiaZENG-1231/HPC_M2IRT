#pragma once

#include "Vector3.hpp"

// One macroparticle in the 1D3V simulation.
struct Particle {
    Vector3 pos; // Position
    Vector3 vel; // Velocity

    double q = 1.0;      // Electric charge
    double m = 1.0;      // Particle mass
    double weight = 1.0; // Number of physical particles represented
};