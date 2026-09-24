#pragma once

#include "Particle.hpp"

// Advance one particle by one time step in given electric and magnetic fields.
void borisPush(Particle& p,
               const Vector3& E,
               const Vector3& B,
               double dt);