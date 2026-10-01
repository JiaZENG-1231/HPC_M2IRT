#include "BorisPusher.hpp"

#include <cmath>
#include <stdexcept>

void borisPush(
    Particle& p,
    const Vector3& E,
    const Vector3& B,
    double dt
) {
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

// Push each particle using electric and magnetic fields
// that have already been evaluated at its position.
void borisPush(
    std::vector<Particle>& particles,
    const std::vector<Vector3>& E_part,
    const std::vector<Vector3>& B_part,
    double dt
) {
    // Every particle must have one electric and one magnetic field.
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

void applyPeriodicBoundary(
    Particle& p,
    const FieldGrid& grid
) {
    if (grid.size() < 2) {
        throw std::invalid_argument(
            "A periodic grid requires at least two grid points."
        );
    }

    const double x_min = grid.xMin();

    // The first and last nodes define the periodic interval.
    const double domain_length =
        grid.spacing() * static_cast<double>(grid.size() - 1);

    if (domain_length <= 0.0) {
        throw std::invalid_argument(
            "The periodic domain length must be positive."
        );
    }

    // fmod also handles particles that cross several domain lengths
    // during a single time step.
    double wrapped_x =
        std::fmod(p.pos.x - x_min, domain_length);

    // C++ fmod returns a negative value for negative input.
    if (wrapped_x < 0.0) {
        wrapped_x += domain_length;
    }

    p.pos.x = x_min + wrapped_x;
}

// Interpolate the local fields, push every particle and return it
// to the periodic x domain when it crosses a boundary.
void borisPush(
    std::vector<Particle>& particles,
    const FieldGrid& grid,
    double dt
) {
    for (Particle& p : particles) {
        // Fields are evaluated at the current particle position.
        const Vector3 E = grid.electricField(p.pos.x);
        const Vector3 B = grid.magneticField(p.pos.x);

        // Advance velocity and position with the Boris algorithm.
        borisPush(p, E, B, dt);

        // Wrap the new x position into the periodic domain.
        applyPeriodicBoundary(p, grid);
    }
}