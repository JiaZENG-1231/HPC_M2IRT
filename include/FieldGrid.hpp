#pragma once

#include <cstddef>
#include <vector>

#include "Vector3.hpp"

// One-dimensional uniform grid containing three-dimensional fields.
class FieldGrid {
public:
    FieldGrid(
        double x_min,
        double dx,
        const std::vector<Vector3>& E_grid,
        const std::vector<Vector3>& B_grid
    );

    // Interpolate the electric field at a given x position.
    Vector3 electricField(double x) const;

    // Interpolate the magnetic field at a given x position.
    Vector3 magneticField(double x) const;

    // Basic grid information used during particle deposition.
    double xMin() const;
    double spacing() const;
    std::size_t size() const;

private:
    double x_min_; // Position of the first grid point
    double dx_;    // Distance between neighbouring grid points

    std::vector<Vector3> E_grid_; // Electric field on grid points
    std::vector<Vector3> B_grid_; // Magnetic field on grid points

    // Linear interpolation of one field array.
    Vector3 interpolate(
        const std::vector<Vector3>& field,
        double x
    ) const;
};