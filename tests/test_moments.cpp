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

bool testMomentValues() {
    const Grid grid{4, 1.0};

    const std::vector<Particle> particles{
        {
            {0.25, 0.0, 0.0},
            {2.00, 0.0, 0.0},
            1.0,
            1.0,
            2.0
        },
        {
            {1.50, 0.0, 0.0},
            {0.00, 2.0, 0.0},
            1.0,
            1.0,
            1.0
        }
    };

    const Moments moments =
        depositMoments(particles, grid);

    const bool density_ok =
        near(moments.density[0], 1.5) &&
        near(moments.density[1], 1.0) &&
        near(moments.density[2], 0.5) &&
        near(moments.density[3], 0.0);

    const bool flux_ok =
        near(moments.flux[0].x, 3.0) &&
        near(moments.flux[0].y, 0.0) &&
        near(moments.flux[1].x, 1.0) &&
        near(moments.flux[1].y, 1.0) &&
        near(moments.flux[2].x, 0.0) &&
        near(moments.flux[2].y, 1.0);

    const bool velocity_ok =
        near(moments.velocity[0].x, 2.0) &&
        near(moments.velocity[0].y, 0.0) &&
        near(moments.velocity[1].x, 1.0) &&
        near(moments.velocity[1].y, 1.0) &&
        near(moments.velocity[2].x, 0.0) &&
        near(moments.velocity[2].y, 2.0);

    return density_ok && flux_ok && velocity_ok;
}

bool testDensityConservation() {
    const Grid grid{4, 0.5};

    const std::vector<Particle> particles{
        {
            {0.10, 0.0, 0.0},
            {},
            1.0,
            1.0,
            2.0
        },
        {
            {1.90, 0.0, 0.0},
            {},
            1.0,
            1.0,
            3.0
        }
    };

    const Moments moments =
        depositMoments(particles, grid);

    double particle_number = 0.0;

    for (double density : moments.density) {
        particle_number += density * grid.spacing();
    }

    return near(particle_number, 5.0);
}

bool testPeriodicDeposition() {
    const Grid grid{4, 1.0};

    const std::vector<Particle> particles{
        {
            {3.75, 0.0, 0.0},
            {},
            1.0,
            1.0,
            1.0
        }
    };

    const Moments moments =
        depositMoments(particles, grid);

    return
        near(moments.density[3], 0.25) &&
        near(moments.density[0], 0.75);
}

} // namespace

int main() {
    const bool values_ok = testMomentValues();
    const bool conservation_ok =
        testDensityConservation();
    const bool periodic_ok =
        testPeriodicDeposition();

    std::cout
        << "Moment values: "
        << (values_ok ? "PASS" : "FAIL")
        << '\n';

    std::cout
        << "Density conservation: "
        << (conservation_ok ? "PASS" : "FAIL")
        << '\n';

    std::cout
        << "Periodic deposition: "
        << (periodic_ok ? "PASS" : "FAIL")
        << '\n';

    const bool all_ok =
        values_ok &&
        conservation_ok &&
        periodic_ok;

    return all_ok ? 0 : 1;
}