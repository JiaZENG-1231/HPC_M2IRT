#include "ParticleMesh.hpp"

#include <cmath>

namespace {

// Indices and linear interpolation weights for one particle.
struct NodeWeights {
    std::size_t left;
    std::size_t right;

    double w_left;
    double w_right;
};

// Find the two grid points surrounding a particle.
NodeWeights getNodeWeights(
    double x,
    const FieldGrid& grid
) {
    const double x_min = grid.xMin();
    const double dx = grid.spacing();

    const std::size_t last = grid.size() - 1;
    const double x_max = x_min + dx * last;

    // Particles outside the grid are assigned to the nearest endpoint.
    if (x <= x_min) {
        return {0, 0, 1.0, 0.0};
    }

    if (x >= x_max) {
        return {last, last, 1.0, 0.0};
    }

    // Particle position measured in units of grid spacing.
    const double grid_pos = (x - x_min) / dx;

    const std::size_t left =
        static_cast<std::size_t>(std::floor(grid_pos));

    const std::size_t right = left + 1;

    // Fractional distance between the left node and the particle.
    const double fraction = grid_pos - left;

    return {
        left,
        right,
        1.0 - fraction,
        fraction
    };
}

} // namespace

std::vector<double> depositCharge(
    const std::vector<Particle>& particles,
    const FieldGrid& grid
) {
    // Charge accumulated at each grid point.
    std::vector<double> node_charge(grid.size(), 0.0);

    for (const Particle& p : particles) {
        const NodeWeights nodes =
            getNodeWeights(p.pos.x, grid);

        // Total charge represented by this macroparticle.
        const double macro_charge = p.q * p.w;

        // Deposit charge on the two neighbouring nodes.
        node_charge[nodes.left] +=
            macro_charge * nodes.w_left;

        node_charge[nodes.right] +=
            macro_charge * nodes.w_right;
    }

    return node_charge;
}

GridMoments computeMoments(
    const std::vector<Particle>& particles,
    const FieldGrid& grid
) {
    GridMoments moments;

    moments.density.assign(grid.size(), 0.0);
    moments.flux.assign(grid.size(), Vector3{});
    moments.velocity.assign(grid.size(), Vector3{});

    for (const Particle& p : particles) {
        const NodeWeights nodes =
            getNodeWeights(p.pos.x, grid);

        // Combine the fixed particle weight with the position-dependent
        // linear interpolation weight.
        const double weight_left =
            p.w * nodes.w_left;

        const double weight_right =
            p.w * nodes.w_right;

        // Deposit the macroparticle statistical weight as number density.
        moments.density[nodes.left] += weight_left;
        moments.density[nodes.right] += weight_right;

        // Deposit weight times velocity as particle flux.
        moments.flux[nodes.left] +=
            weight_left * p.vel;

        moments.flux[nodes.right] +=
            weight_right * p.vel;
    }

    // Calculate bulk velocity only at grid points containing particles.
    for (std::size_t i = 0; i < grid.size(); ++i) {
        if (moments.density[i] > 0.0) {
            moments.velocity[i] =
                moments.flux[i] / moments.density[i];
        }
    }

    return moments;
}