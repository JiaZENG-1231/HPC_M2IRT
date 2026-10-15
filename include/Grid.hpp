#pragma once

#include <cstddef>

// Position type on the grid.
enum class Centering {
    Node,
    CellCenter
};

// Linear weights around a particle.
struct ShapeWeights {
    std::size_t left;
    std::size_t right;

    double w_left;
    double w_right;
};

// Uniform periodic grid along x.
class Grid {
public:
    Grid(
        std::size_t nx,
        double dx
    );

    std::size_t size() const;
    double spacing() const;
    double length() const;

    double nodePosition(
        std::size_t i
    ) const;

    double cellCenterPosition(
        std::size_t i
    ) const;

    // Map x into the interval [0, L).
    double wrap(double x) const;

    // Get the two linear weights.
    ShapeWeights weights(
        double x,
        Centering centering
    ) const;

private:
    std::size_t nx_; // Number of grid points
    double dx_;      // Grid spacing
};