#include <cmath>
#include <iostream>

#include "BorisPusher.hpp"

bool near(
    double a,
    double b,
    double tol = 1.0e-12
) {
    return std::abs(a - b) < tol;
}

bool testFreeMotion() {
    Particle p{
        {0.0, 0.0, 0.0}, // Position
        {1.0, 2.0, 0.0}, // Velocity
        1.0,              // Charge
        1.0               // Mass
    };

    const Vector3 E{};
    const Vector3 B{};

    borisPush(p, E, B, 0.1);

    return
        near(p.pos.x, 0.1) &&
        near(p.pos.y, 0.2) &&
        near(p.pos.z, 0.0) &&
        near(p.vel.x, 1.0) &&
        near(p.vel.y, 2.0) &&
        near(p.vel.z, 0.0);
}

bool testElectricAcceleration() {
    Particle p{
        {0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0},
        1.0,
        1.0
    };

    const Vector3 E{1.0, 0.0, 0.0};
    const Vector3 B{};

    borisPush(p, E, B, 0.1);

    // For q = m = 1, the acceleration equals E.
    return
        near(p.vel.x, 0.1) &&
        near(p.vel.y, 0.0) &&
        near(p.vel.z, 0.0);
}

bool testMagneticEnergy() {
    Particle p{
        {0.0, 0.0, 0.0},
        {1.0, 0.0, 0.0},
        1.0,
        1.0
    };

    const Vector3 E{};
    const Vector3 B{0.0, 0.0, 1.0};

    const double speed_init = p.vel.norm();

    for (int step = 0; step < 1000; ++step) {
        borisPush(p, E, B, 0.01);
    }

    const double speed_final = p.vel.norm();

    // A magnetic field changes direction but does no work.
    return near(speed_final, speed_init, 
1.0e-10);
}

int main() {
    const bool free_ok =
        testFreeMotion();

    const bool electric_ok =
        testElectricAcceleration();

    const bool magnetic_ok =
        testMagneticEnergy();

    std::cout << "Free motion: "
              << (free_ok ? "PASS" : "FAIL")
              << '\n';

    std::cout << "Electric acceleration: "
              << (electric_ok ? "PASS" : "FAIL")
              << '\n';

    std::cout << "Magnetic energy: "
              << (magnetic_ok ? "PASS" : "FAIL")
              << '\n';

    const bool tests_ok =
        free_ok &&
        electric_ok &&
        magnetic_ok;

    return tests_ok ? 0 : 1;
}
