#include <cmath>
#include <iostream>

#include "Vector3.hpp"

bool near(
    double a,
    double b,
    double tol = 1.0e-12
) {
    return std::abs(a - b) < tol;
}

int main() {
    // Unit vectors along x and y.
    const Vector3 x_unit{1.0, 0.0, 0.0};
    const Vector3 y_unit{0.0, 1.0, 0.0};

    const Vector3 sum = x_unit + y_unit;
    const Vector3 diff = x_unit - y_unit;
    const Vector3 scaled = 2.0 * x_unit;

    const bool add_ok =
        near(sum.x, 1.0) &&
        near(sum.y, 1.0) &&
        near(sum.z, 0.0);

    const bool subtract_ok =
        near(diff.x, 1.0) &&
        near(diff.y, -1.0) &&
        near(diff.z, 0.0);

    const bool scale_ok =
        near(scaled.x, 2.0) &&
        near(scaled.y, 0.0) &&
        near(scaled.z, 0.0);

    // Perpendicular unit vectors have zero dot product.
    const bool dot_ok =
        near(dot(x_unit, y_unit), 0.0);

    const Vector3 cross_val =
        cross(x_unit, y_unit);

    // x cross y points along positive z.
    const bool cross_ok =
        near(cross_val.x, 0.0) &&
        near(cross_val.y, 0.0) &&
        near(cross_val.z, 1.0);

    const bool norm_ok =
        near(x_unit.norm(), 1.0) &&
        near(x_unit.norm2(), 1.0);

    std::cout << "Vector addition: "
              << (add_ok ? "PASS" : "FAIL")
              << '\n';

    std::cout << "Vector subtraction: "
              << (subtract_ok ? "PASS" : "FAIL")
              << '\n';

    std::cout << "Scalar multiplication: "
              << (scale_ok ? "PASS" : "FAIL")
              << '\n';

    std::cout << "Dot product: "
              << (dot_ok ? "PASS" : "FAIL")
              << '\n';

    std::cout << "Cross product: "
              << (cross_ok ? "PASS" : "FAIL")
              << '\n';

    std::cout << "Vector norm: "
              << (norm_ok ? "PASS" : "FAIL")
              << '\n';

    const bool tests_ok =
        add_ok &&
        subtract_ok &&
        scale_ok &&
        dot_ok &&
        cross_ok &&
        norm_ok;

    return tests_ok ? 0 : 1;
}
