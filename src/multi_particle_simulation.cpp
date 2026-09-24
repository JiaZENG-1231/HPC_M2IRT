#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "BorisPusher.hpp"

int main() {
    // Particles have different initial speeds or charge signs.
    std::vector<Particle> particles{
        {
            {0.0, 0.0, 0.0}, // Initial position
            {0.5, 0.0, 0.0}, // Initial velocity
            1.0,              // Charge
            1.0               // Mass
        },
        {
            {0.0, 0.0, 0.0},
            {1.0, 0.0, 0.0},
            1.0,
            1.0
        },
        {
            {0.0, 0.0, 0.0},
            {1.5, 0.0, 0.0},
            1.0,
            1.0
        },
        {
            {0.0, 0.0, 0.0},
            {1.0, 0.0, 0.0},
            -1.0,
            1.0
        }
    };

    // A constant magnetic field is applied along z.
    const Vector3 E{0.0, 0.0, 0.0};
    const Vector3 B{0.0, 0.0, 1.0};

    const double dt = 0.02;
    const int n_steps = 1000;

    std::ofstream output{
        "results/multi_particle_motion.csv"
    };

    if (!output) {
        std::cerr << "Could not open the output file.\n";
        return 1;
    }

    output << std::setprecision(15);

    output << "time,particle,"
           << "x,y,z,"
           << "vx,vy,vz,"
           << "speed,"
           << "q,m,"
           << "Ex,Ey,Ez,"
           << "Bx,By,Bz\n";

    for (int step = 0; step <= n_steps; ++step) {
        const double time = step * dt;

        // Save the state of every particle at the current time.
        for (std::size_t i = 0; i < particles.size(); ++i) {
            const Particle& p = particles[i];

            output << time << ','
                   << i << ','
                   << p.pos.x << ','
                   << p.pos.y << ','
                   << p.pos.z << ','
                   << p.vel.x << ','
                   << p.vel.y << ','
                   << p.vel.z << ','
                   << p.vel.norm() << ','
                   << p.q << ','
                   << p.m << ','
                   << E.x << ','
                   << E.y << ','
                   << E.z << ','
                   << B.x << ','
                   << B.y << ','
                   << B.z << '\n';
        }

        if (step < n_steps) {
            // Advance all particles with one call.
            borisPush(particles, E, B, dt);
        }
    }

    output.close();

    std::cout << "Multi-particle simulation completed.\n";
    std::cout
        << "Results saved in results/multi_particle_motion.csv\n";

    for (std::size_t i = 0; i < particles.size(); ++i) {
        std::cout << "Particle " << i
                  << ": position = " << particles[i].pos
                  << ", speed = " << particles[i].vel.norm()
                  << '\n';
    }

    return 0;
}