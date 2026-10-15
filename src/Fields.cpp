#include "Fields.hpp"

namespace {

double gatherComponent(
    const std::vector<double>& values,
    const ShapeWeights& w
) {
    return
        w.w_left * values[w.left] +
        w.w_right * values[w.right];
}

} // namespace

VectorField::VectorField(
    std::size_t nx,
    Centering x_pos,
    Centering y_pos,
    Centering z_pos
)
    : x(nx, 0.0),
      y(nx, 0.0),
      z(nx, 0.0),
      x_center(x_pos),
      y_center(y_pos),
      z_center(z_pos) {
}

VectorField makeElectricField(
    const Grid& grid
) {
    return {
        grid.size(),
        Centering::CellCenter,
        Centering::Node,
        Centering::Node
    };
}

VectorField makeMagneticField(
    const Grid& grid
) {
    return {
        grid.size(),
        Centering::Node,
        Centering::CellCenter,
        Centering::CellCenter
    };
}

Vector3 gatherField(
    const VectorField& field,
    const Grid& grid,
    double x
) {
    const ShapeWeights x_w =
        grid.weights(
            x,
            field.x_center
        );

    const ShapeWeights y_w =
        grid.weights(
            x,
            field.y_center
        );

    const ShapeWeights z_w =
        grid.weights(
            x,
            field.z_center
        );

    return {
        gatherComponent(field.x, x_w),
        gatherComponent(field.y, y_w),
        gatherComponent(field.z, z_w)
    };
}