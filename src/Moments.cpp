#include "Moments.hpp"

Moments depositMoments(
    const std::vector<Particle>& particles,
    const Grid& grid
) {
    Moments moments;

    // All particle moments are stored on grid nodes.
    moments.density.assign(grid.size(), 0.0);
    moments.flux.assign(grid.size(), Vector3{});
    moments.velocity.assign(grid.size(), Vector3{});

    for (const Particle& p : particles) {
        const ShapeWeights shape =
            grid.weights(p.pos.x, Centering::Node);

        // Convert the particle weight into a 1D density.
        const double left_density =
            p.weight * shape.w_left / grid.spacing();

        const double right_density =
            p.weight * shape.w_right / grid.spacing();

        // Deposit density on the two neighbouring nodes.
        moments.density[shape.left] += left_density;
        moments.density[shape.right] += right_density;

        // Deposit density multiplied by particle velocity.
        moments.flux[shape.left] +=
            left_density * p.vel;

        moments.flux[shape.right] +=
            right_density * p.vel;
    }

    // Recover the mean velocity from flux divided by density.
    for (std::size_t i = 0; i < grid.size(); ++i) {
        if (moments.density[i] > 0.0) {
            moments.velocity[i] =
                moments.flux[i] / moments.density[i];
        }
    }

    return moments;
}