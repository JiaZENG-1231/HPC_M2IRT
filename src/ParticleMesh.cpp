#include "ParticleMesh.hpp"

#include <cmath>

namespace {

// Grid indices and interpolation weights surrounding one particle.
struct NodeWeights {
    std::size_t left;
    std::size_t right;

    double w_left;
    double w_right;
};

// Find the two nodes surrounding a particle.
NodeWeights getNodeWeights(
    double x,
    const FieldGrid& grid
) {
    const double x_min = grid.xMin();
    const double dx = grid.spacing();

    const std::size_t last = grid.size() - 1;
    const double x_max = x_min + dx * last;

    // Assign particles outside the grid to the nearest endpoint.
    if (x <= x_min) {
        return {0, 0, 1.0, 0.0};
    }

    if (x >= x_max) {
        return {last, last, 1.0, 0.0};
    }

    // Position measured in units of grid spacing.
    const double grid_pos = (x - x_min) / dx;

    const std::size_t left =
        static_cast<std::size_t>(std::floor(grid_pos));

    const std::size_t right = left + 1;

    // Fractional distance from the left grid point.
    const double weight = grid_pos - left;

    return {
        left,
        right,
        1.0 - weight,
        weight
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

        // Order-1 B-spline deposition.
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
    moments.velocity.assign(grid.size(), Vector3{});

    for (const Particle& p : particles) {
        const NodeWeights nodes =
            getNodeWeights(p.pos.x, grid);

        // Statistical weight deposited on each neighbouring node.
        const double weight_left =
            p.w * nodes.w_left;

        const double weight_right =
            p.w * nodes.w_right;

        // Accumulate particle number on the grid.
        moments.density[nodes.left] += weight_left;
        moments.density[nodes.right] += weight_right;

        // Accumulate weighted velocity before normalization.
        moments.velocity[nodes.left] +=
            weight_left * p.vel;

        moments.velocity[nodes.right] +=
            weight_right * p.vel;
    }

    // Convert weighted velocity sums into mean velocities.
    for (std::size_t i = 0; i < grid.size(); ++i) {
        if (moments.density[i] > 0.0) {
            moments.velocity[i] =
                moments.velocity[i] / moments.density[i];
        }
    }

    return moments;
}