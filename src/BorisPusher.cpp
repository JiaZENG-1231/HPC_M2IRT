#include "BorisPusher.hpp"
#include <stdexcept>

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

// Apply the single-particle Boris pusher to every particle.
void borisPush(
    std::vector<Particle>& particles,
    const Vector3& E,
    const Vector3& B,
    double dt
) {
    for (Particle& p : particles) {
        borisPush(p, E, B, dt);
    }
}

// Push each particle using the electric and magnetic fields
// evaluated at its own position.
void borisPush(
    std::vector<Particle>& particles,
    const std::vector<Vector3>& E_part,
    const std::vector<Vector3>& B_part,
    double dt
) {
    // Each particle must have one electric and one magnetic field.
    if (particles.size() != E_part.size() ||
        particles.size() != B_part.size()) {
        throw std::invalid_argument(
            "Particle and field arrays must have the same size."
        );
    }

    for (std::size_t i = 0; i < particles.size(); ++i) {
        borisPush(particles[i], E_part[i], B_part[i], dt);
    }
}

// Interpolate the local fields and push every particle.
void borisPush(
    std::vector<Particle>& particles,
    const FieldGrid& grid,
    double dt
) {
    for (Particle& p : particles) {
        // Fields are evaluated at the particle's current position.
        const Vector3 E = grid.electricField(p.pos.x);
        const Vector3 B = grid.magneticField(p.pos.x);

        // Advance the particle using its local fields.
        borisPush(p, E, B, dt);
    }
}