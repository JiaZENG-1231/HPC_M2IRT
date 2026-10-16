#include <cmath>
#include <iostream>
#include <vector>

#include "Moments.hpp"

namespace {

bool near(
    double a,
    double b,
    double tol = 1.0e-12
) {
    return std::abs(a - b) < tol;
}

bool testCounterStreamingParticles() {
    const Grid grid{4, 1.0};

    // Equal and opposite velocities give zero bulk flow.
    const std::vector<Particle> particles{
        {
            {1.0, 0.0, 0.0},
            {1.0, 0.0, 0.0},
            1.0,
            1.0,
            1.0
        },
        {
            {1.0, 0.0, 0.0},
            {-1.0, 0.0, 0.0},
            1.0,
            1.0,
            1.0
        }
    };

    const Moments moments =
        depositMoments(particles, grid);

    const bool density_ok =
        near(moments.density[1], 2.0);

    const bool velocity_ok =
        near(moments.velocity[1].x, 0.0) &&
        near(moments.velocity[1].y, 0.0) &&
        near(moments.velocity[1].z, 0.0);

    // Each particle has kinetic energy 1/2.
    const bool energy_ok =
        near(moments.energy[1], 1.0);

    // T = 2 E_th / (3 n) = 1/3.
    const bool temperature_ok =
        near(moments.temperature[1], 1.0 / 3.0);

    return
        density_ok &&
        velocity_ok &&
        energy_ok &&
        temperature_ok;
}

bool testColdBulkFlow() {
    const Grid grid{4, 1.0};

    // Identical velocities represent bulk flow
    // without a thermal velocity spread.
    const std::vector<Particle> particles{
        {
            {2.0, 0.0, 0.0},
            {2.0, 0.0, 0.0},
            1.0,
            1.0,
            1.0
        },
        {
            {2.0, 0.0, 0.0},
            {2.0, 0.0, 0.0},
            1.0,
            1.0,
            1.0
        }
    };

    const Moments moments =
        depositMoments(particles, grid);

    const bool velocity_ok =
        near(moments.velocity[2].x, 2.0);

    const bool energy_ok =
        near(moments.energy[2], 4.0);

    const bool temperature_ok =
        near(moments.temperature[2], 0.0);

    return
        velocity_ok &&
        energy_ok &&
        temperature_ok;
}

} // namespace

int main() {
    const bool thermal_ok =
        testCounterStreamingParticles();

    const bool cold_flow_ok =
        testColdBulkFlow();

    std::cout
        << "Thermal velocity spread: "
        << (thermal_ok ? "PASS" : "FAIL")
        << '\n';

    std::cout
        << "Cold bulk flow: "
        << (cold_flow_ok ? "PASS" : "FAIL")
        << '\n';

    return thermal_ok && cold_flow_ok ? 0 : 1;
}