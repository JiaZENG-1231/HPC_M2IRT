#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "BorisPusher.hpp"

int main() {
    // A positively charged particle starts with velocity along x.
    Particle p{
        {0.0, 0.0, 0.0}, // Initial position
        {1.0, 0.0, 0.0}, // Initial velocity
        1.0,              // Charge
        1.0               // Mass
    };

    // A magnetic field along z produces circular motion in the x-y plane.
    const Vector3 E{0.0, 0.0, 0.0};
    const Vector3 B{0.0, 0.0, 1.0};

    const double dt = 0.02;
    const int n_steps = 1000;

    // Store positions, velocities and fields for later plotting.
    std::ofstream output{"results/particle_motion.csv"};

    if (!output) {
        std::cerr << "Could not open the output file.\n";
        return 1;
    }

    output << std::setprecision(15);

    output << "time,"
           << "x,y,z,"
           << "vx,vy,vz,"
           << "speed,"
           << "Ex,Ey,Ez,"
           << "Bx,By,Bz\n";

    for (int step = 0; step <= n_steps; ++step) {
        const double time = step * dt;
        const double speed = p.vel.norm();

        // Save the current particle and field values.
        output << time << ','
               << p.pos.x << ','
               << p.pos.y << ','
               << p.pos.z << ','
               << p.vel.x << ','
               << p.vel.y << ','
               << p.vel.z << ','
               << speed << ','
               << E.x << ','
               << E.y << ','
               << E.z << ','
               << B.x << ','
               << B.y << ','
               << B.z << '\n';

        if (step < n_steps) {
            borisPush(p, E, B, dt);
        }
    }

    output.close();

    std::cout << "Simulation completed.\n";
    std::cout << "Results saved in results/particle_motion.csv\n";
    std::cout << "Final position: " << p.pos << '\n';
    std::cout << "Final velocity: " << p.vel << '\n';
    std::cout << "Final speed: " << p.vel.norm() << '\n';

    return 0;
}