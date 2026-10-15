#include <fstream>
#include <iostream>

#include "BorisPusher.hpp"

int main() {
    Particle p{
        {0.0, 0.0, 0.0}, // Initial position
        {1.0, 0.0, 0.0}, // Initial velocity
        1.0,              // Charge
        1.0               // Mass
    };

    const Vector3 E{};
    const Vector3 B{0.0, 0.0, 1.0};

    const double dt = 0.01;
    const int n_steps = 630;

    std::ofstream file{
        "results/particle_trajectory.csv"
    };

    if (!file) {
        std::cerr << "Could not open the output file.\n";
        return 1;
    }

    file << "time,x,y,z,vx,vy,vz,speed\n";

    // Save the initial state before the first time step.
    file << 0.0 << ','
         << p.pos.x << ','
         << p.pos.y << ','
         << p.pos.z << ','
         << p.vel.x << ','
         << p.vel.y << ','
         << p.vel.z << ','
         << p.vel.norm() << '\n';

    for (int step = 1; step <= n_steps; ++step) {
        borisPush(p, E, B, dt);

        const double time = step * dt;

        file << time << ','
             << p.pos.x << ','
             << p.pos.y << ','
             << p.pos.z << ','
             << p.vel.x << ','
             << p.vel.y << ','
             << p.vel.z << ','
             << p.vel.norm() << '\n';
    }

    std::cout
        << "Saved results/particle_trajectory.csv\n";

    return 0;
}