#include "BorisPusher.hpp"

void borisPush(Particle& p,
               const Vector3& E,
               const Vector3& B,
               double dt) {
    // Half of the acceleration produced by the electric field.
    const double half_kick = p.q * dt / (2.0 * p.m);

    // First half-step electric acceleration.
    const Vector3 v_minus = p.vel + half_kick * E;

    // Rotation vectors associated with the magnetic field.
    const Vector3 t = half_kick * B;
    const Vector3 s = 2.0 * t / (1.0 + t.norm2());

    // Rotate the velocity without changing its magnitude.
    const Vector3 v_prime = v_minus + cross(v_minus, t);
    const Vector3 v_plus = v_minus + cross(v_prime, s);

    // Second half-step electric acceleration.
    p.vel = v_plus + half_kick * E;

    // Advance the position using the new half-step velocity.
    p.pos += dt * p.vel;
}