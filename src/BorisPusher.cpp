#include "BorisPusher.hpp"

#include <stdexcept>

#include "ParticleBoundary.hpp"

void borisPush(
    Particle& p,
    const Vector3& E,
    const Vector3& B,
    double dt
) {
    if (p.m <= 0.0) {
        throw std::invalid_argument(
            "Particle mass must be positive."
        );
    }

    const double half_kick =
        p.q * dt / (2.0 * p.m);

    // First half of the electric acceleration.
    const Vector3 v_minus =
        p.vel + half_kick * E;

    // Magnetic rotation.
    const Vector3 t =
        half_kick * B;

    const Vector3 s =
        2.0 * t / (1.0 + t.norm2());

    const Vector3 v_prime =
        v_minus + cross(v_minus, t);

    const Vector3 v_plus =
        v_minus + cross(v_prime, s);

    // Second half of the electric acceleration.
    p.vel =
        v_plus + half_kick * E;

    p.pos += dt * p.vel;
}

void borisPush(
    std::vector<Particle>& particles,
    const Vector3& E,
    const Vector3& B,
    double dt
) {
    // Apply the single-particle algorithm to the full population.
    for (Particle& p : particles) {
        borisPush(p, E, B, dt);
    }
}


void borisPush(
    std::vector<Particle>& particles,
    const VectorField& E,
    const VectorField& B,
    const Grid& grid,
    double dt
) {
    for (Particle& p : particles) {
        // Evaluate the fields at the current position.
        const Vector3 E_part =
            gatherField(E, grid, p.pos.x);

        const Vector3 B_part =
            gatherField(B, grid, p.pos.x);

        borisPush(
            p,
            E_part,
            B_part,
            dt
        );

        applyPeriodicBoundary(
            p,
            grid
        );
    }
}