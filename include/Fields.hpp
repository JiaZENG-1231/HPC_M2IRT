#pragma once

#include <vector>

#include "Grid.hpp"
#include "Vector3.hpp"

// Three field components on a staggered grid.
struct VectorField {
    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> z;

    Centering x_center;
    Centering y_center;
    Centering z_center;

    VectorField(
        std::size_t nx,
        Centering x_pos,
        Centering y_pos,
        Centering z_pos
    );
};

// Create a field with the electric Yee layout.
VectorField makeElectricField(
    const Grid& grid
);

// Create a field with the magnetic Yee layout.
VectorField makeMagneticField(
    const Grid& grid
);

// Interpolate all three components at position x.
Vector3 gatherField(
    const VectorField& field,
    const Grid& grid,
    double x
);