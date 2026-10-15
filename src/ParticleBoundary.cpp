#include "ParticleBoundary.hpp"

void applyPeriodicBoundary(
    Particle& p,
    const Grid& grid
) {
    p.pos.x = grid.wrap(p.pos.x);
}

void applyPeriodicBoundary(
    std::vector<Particle>& particles,
    const Grid& grid
) {
    for (Particle& p : particles) {
        applyPeriodicBoundary(p, grid);
    }
}