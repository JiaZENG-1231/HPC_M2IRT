#pragma once

#include "Vector3.hpp"

// Physical state of one charged particle.
struct Particle {
    Vector3 pos;   // Position at the current integer time step.
    Vector3 vel;   // Velocity stored at a half time step.

    double q = 1.0;   // Electric charge.
    double m = 1.0;   // Particle mass.

    Particle() = default;

    Particle(const Vector3& pos_init,
             const Vector3& vel_init,
             double q_val,
             double m_val)
        : pos(pos_init),
          vel(vel_init),
          q(q_val),
          m(m_val) {}
};