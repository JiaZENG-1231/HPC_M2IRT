#include <cmath>
#include <iostream>
#include <vector>

#include "ParticleBoundary.hpp"

bool near(
    double a,
    double b,
    double tol = 1.0e-12
) {
    return std::abs(a - b) < tol;
}

bool testRightBoundary() {
    const Grid grid{4, 1.0};

    Particle p{
        {4.25, 1.0, 2.0},
        {1.0, 2.0, 3.0},
        1.0,
        1.0
    };

    applyPeriodicBoundary(p, grid);

    return
        near(p.pos.x, 0.25) &&
        near(p.pos.y, 1.0) &&
        near(p.pos.z, 2.0) &&
        near(p.vel.x, 1.0) &&
        near(p.vel.y, 2.0) &&
        near(p.vel.z, 3.0);
}

bool testLeftBoundary() {
    const Grid grid{4, 1.0};

    Particle p{
        {-0.25, 0.0, 0.0},
        {-1.0, 0.0, 0.0},
        1.0,
        1.0
    };

    applyPeriodicBoundary(p, grid);

    return near(p.pos.x, 3.75);
}

bool testSeveralCrossings() {
    const Grid grid{4, 1.0};

    Particle right{
        {12.25, 0.0, 0.0},
        {},
        1.0,
        1.0
    };

    Particle left{
        {-8.25, 0.0, 0.0},
        {},
        1.0,
        1.0
    };

    applyPeriodicBoundary(right, grid);
    applyPeriodicBoundary(left, grid);

    return
        near(right.pos.x, 0.25) &&
        near(left.pos.x, 3.75);
}

bool testParticleArray() {
    const Grid grid{4, 1.0};

    std::vector<Particle> particles{
        {
            {4.50, 0.0, 0.0},
            {},
            1.0,
            1.0
        },
        {
            {-0.50, 0.0, 0.0},
            {},
            1.0,
            1.0
        }
    };

    applyPeriodicBoundary(
        particles,
        grid
    );

    return
        near(particles[0].pos.x, 0.50) &&
        near(particles[1].pos.x, 3.50);
}

int main() {
    const bool right_ok =
        testRightBoundary();

    const bool left_ok =
        testLeftBoundary();

    const bool crossings_ok =
        testSeveralCrossings();

    const bool array_ok =
        testParticleArray();

    std::cout << "Right boundary: "
              << (right_ok ? "PASS" : "FAIL")
              << '\n';

    std::cout << "Left boundary: "
              << (left_ok ? "PASS" : "FAIL")
              << '\n';

    std::cout << "Several crossings: "
              << (crossings_ok ? "PASS" : "FAIL")
              << '\n';

    std::cout << "Particle array: "
              << (array_ok ? "PASS" : "FAIL")
              << '\n';

    const bool tests_ok =
        right_ok &&
        left_ok &&
        crossings_ok &&
        array_ok;

    return tests_ok ? 0 : 1;
}