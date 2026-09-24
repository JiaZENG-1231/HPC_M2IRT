#include <cmath>
#include <iostream>

#include "BorisPusher.hpp"

// Compare two floating-point numbers within a given tolerance.
bool near(double a, double b, double tol = 1.0e-12) {
    return std::abs(a - b) < tol;
}

// Check the analytical solution for motion without fields.
bool testFreeMotion() {
    Particle p{
        {0.0, 0.0, 0.0},   // Initial position
        {1.0, 2.0, 0.0},   // Constant velocity
        1.0,
        1.0
    };

    const Vector3 E{0.0, 0.0, 0.0};
    const Vector3 B{0.0, 0.0, 0.0};
    const double dt = 0.1;

    borisPush(p, E, B, dt);

    const bool pos_ok =
        near(p.pos.x, 0.1) &&
        near(p.pos.y, 0.2) &&
        near(p.pos.z, 0.0);

    const bool vel_ok =
        near(p.vel.x, 1.0) &&
        near(p.vel.y, 2.0) &&
        near(p.vel.z, 0.0);

    return pos_ok && vel_ok;
}

// A magnetic field rotates the velocity but performs no work.
// Therefore, the speed must remain constant.
bool testMagneticEnergy() {
    Particle p{
        {0.0, 0.0, 0.0},
        {1.0, 0.0, 0.0},
        1.0,
        1.0
    };

    const Vector3 E{0.0, 0.0, 0.0};
    const Vector3 B{0.0, 0.0, 1.0};

    const double dt = 0.01;
    const int n_steps = 1000;

    const double speed_init = p.vel.norm();

    for (int step = 0; step < n_steps; ++step) {
        borisPush(p, E, B, dt);
    }

    const double speed_final = p.vel.norm();

    return near(speed_final, speed_init, 1.0e-10);
}

int main() {
    const bool free_motion_ok = testFreeMotion();
    const bool magnetic_energy_ok = testMagneticEnergy();

    std::cout << "Free motion: "
              << (free_motion_ok ? "PASS" : "FAIL") << '\n';

    std::cout << "Magnetic energy conservation: "
              << (magnetic_energy_ok ? "PASS" : "FAIL") << '\n';

    return (free_motion_ok && magnetic_energy_ok) ? 0 : 1;
}