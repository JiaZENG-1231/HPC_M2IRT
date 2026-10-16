#include "Moments.hpp"

Moments depositMoments(
    const std::vector<Particle>& particles,
    const Grid& grid
) {
    Moments moments;

    moments.density.assign(grid.size(), 0.0);
    moments.flux.assign(grid.size(), Vector3{});
    moments.velocity.assign(grid.size(), Vector3{});
    moments.energy.assign(grid.size(), 0.0);
    moments.temperature.assign(grid.size(), 0.0);

    // Mass density is needed to separate bulk
    // and thermal kinetic energy.
    std::vector<double> mass_density(
        grid.size(),
        0.0
    );

    for (const Particle& p : particles) {
        const ShapeWeights shape =
            grid.weights(p.pos.x, Centering::Node);

        // CIC density contributions.
        const double n_left =
            p.weight * shape.w_left / grid.spacing();

        const double n_right =
            p.weight * shape.w_right / grid.spacing();

        moments.density[shape.left] += n_left;
        moments.density[shape.right] += n_right;

        // Deposit particle flux.
        moments.flux[shape.left] += n_left * p.vel;
        moments.flux[shape.right] += n_right * p.vel;

        // Deposit mass density.
        mass_density[shape.left] += n_left * p.m;
        mass_density[shape.right] += n_right * p.m;

        // Kinetic energy of one physical particle.
        const double kinetic_energy =
            0.5 * p.m * p.vel.norm2();

        moments.energy[shape.left] +=
            n_left * kinetic_energy;

        moments.energy[shape.right] +=
            n_right * kinetic_energy;
    }

    for (std::size_t i = 0; i < grid.size(); ++i) {
        if (moments.density[i] <= 0.0) {
            continue;
        }

        // Mean velocity is flux divided by density.
        moments.velocity[i] =
            moments.flux[i] / moments.density[i];

        const double bulk_energy =
            0.5 *
            mass_density[i] *
            moments.velocity[i].norm2();

        const double thermal_energy =
            moments.energy[i] - bulk_energy;

        // Three velocity dimensions give 3 n T / 2.
        if (thermal_energy > 0.0) {
            moments.temperature[i] =
                2.0 * thermal_energy /
                (3.0 * moments.density[i]);
        }
    }

    return moments;
}