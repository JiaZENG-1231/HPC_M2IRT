#include <iostream>

#include "BorisPusher.hpp"

int main() {
    // A unit-mass, unit-charge particle moving along the x direction.
    Particle p{
        {0.0, 0.0, 0.0},   // Initial position
        {1.0, 0.0, 0.0},   // Initial velocity
        1.0,               // Charge
        1.0                // Mass
    };

    // With no fields, the velocity must remain constant.
    const Vector3 E{0.0, 0.0, 0.0};
    const Vector3 B{0.0, 0.0, 0.0};

    const double dt = 0.1;

    borisPush(p, E, B, dt);

    std::cout << "position = " << p.pos << '\n';
    std::cout << "velocity = " << p.vel << '\n';

    return 0;
}