#pragma once

#include "Vector3.hpp"

// Physical state of one simulation particle.
struct Particle {
    Vector3 pos; // Position at the current time step
    Vector3 vel; // Velocity stored at the half time step

    double q = 1.0; // Charge of one physical particle
    double m = 1.0; // Mass of one physical particle

    // Number of physical particles represented by this macroparticle.
    double w = 1.0;

    Particle() = default;

    Particle(
        const Vector3& pos_init,
        const Vector3& vel_init,
        double q_val,
        double m_val,
        double w_val = 1.0
    )
        : pos(pos_init),
          vel(vel_init),
          q(q_val),
          m(m_val),
          w(w_val) {}
};