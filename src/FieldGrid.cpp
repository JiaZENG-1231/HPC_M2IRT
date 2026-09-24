#include "FieldGrid.hpp"

#include <cmath>
#include <stdexcept>

FieldGrid::FieldGrid(
    double x_min,
    double dx,
    const std::vector<Vector3>& E_grid,
    const std::vector<Vector3>& B_grid
)
    : x_min_(x_min),
      dx_(dx),
      E_grid_(E_grid),
      B_grid_(B_grid) {
    // Interpolation requires at least two grid points.
    if (E_grid_.size() < 2) {
        throw std::invalid_argument(
            "The field grid requires at least two points."
        );
    }

    if (E_grid_.size() != B_grid_.size()) {
        throw std::invalid_argument(
            "Electric and magnetic grids must have the same size."
        );
    }

    if (dx_ <= 0.0) {
        throw std::invalid_argument(
            "Grid spacing must be positive."
        );
    }
}

Vector3 FieldGrid::electricField(double x) const {
    return interpolate(E_grid_, x);
}

Vector3 FieldGrid::magneticField(double x) const {
    return interpolate(B_grid_, x);
}

Vector3 FieldGrid::interpolate(
    const std::vector<Vector3>& field,
    double x
) const {
    const std::size_t last = field.size() - 1;
    const double x_max = x_min_ + dx_ * last;

    // Use the nearest endpoint outside the grid.
    if (x <= x_min_) {
        return field.front();
    }

    if (x >= x_max) {
        return field.back();
    }

    // Convert the physical position into a grid coordinate.
    const double grid_pos = (x - x_min_) / dx_;
    const std::size_t left =
        static_cast<std::size_t>(std::floor(grid_pos));

    const std::size_t right = left + 1;

    // Fractional distance from the left grid point.
    const double weight = grid_pos - left;

    // Linear interpolation between the two neighbouring points.
    return (1.0 - weight) * field[left]
         + weight * field[right];
}

double FieldGrid::xMin() const {
    return x_min_;
}

double FieldGrid::spacing() const {
    return dx_;
}

std::size_t FieldGrid::size() const {
    return E_grid_.size();
}