#include "Grid.hpp"

#include <cmath>
#include <stdexcept>

Grid::Grid(
    std::size_t nx,
    double dx
)
    : nx_(nx),
      dx_(dx) {
    if (nx_ < 2) {
        throw std::invalid_argument(
            "Grid needs at least two points."
        );
    }

    if (dx_ <= 0.0) {
        throw std::invalid_argument(
            "Grid spacing must be positive."
        );
    }
}

std::size_t Grid::size() const {
    return nx_;
}

double Grid::spacing() const {
    return dx_;
}

double Grid::length() const {
    return static_cast<double>(nx_) * dx_;
}

double Grid::nodePosition(
    std::size_t i
) const {
    if (i >= nx_) {
        throw std::out_of_range(
            "Node index is out of range."
        );
    }

    return static_cast<double>(i) * dx_;
}

double Grid::cellCenterPosition(
    std::size_t i
) const {
    if (i >= nx_) {
        throw std::out_of_range(
            "Cell index is out of range."
        );
    }

    return (
        static_cast<double>(i) + 0.5
    ) * dx_;
}

double Grid::wrap(double x) const {
    double x_new =
        std::fmod(x, length());

    if (x_new < 0.0) {
        x_new += length();
    }

    return x_new;
}

ShapeWeights Grid::weights(
    double x,
    Centering centering
) const {
    x = wrap(x);

    const double shift =
        centering == Centering::CellCenter
        ? 0.5
        : 0.0;

    const double grid_pos =
        x / dx_ - shift;

    const double left_val =
        std::floor(grid_pos);

    const auto left_raw =
        static_cast<long long>(left_val);

    const double w_right =
        grid_pos - left_val;

    const auto nx =
        static_cast<long long>(nx_);

    // Convert to a periodic index.
    const auto left =
        (left_raw % nx + nx) % nx;

    const auto right =
        (left + 1) % nx;

    return {
        static_cast<std::size_t>(left),
        static_cast<std::size_t>(right),
        1.0 - w_right,
        w_right
    };
}