#include <cmath>
#include <iostream>

#include "Fields.hpp"

bool near(
    double a,
    double b,
    double tol = 1.0e-12
) {
    return std::abs(a - b) < tol;
}

bool testFieldLayout() {
    const Grid grid{4, 1.0};

    const VectorField E =
        makeElectricField(grid);

    const VectorField B =
        makeMagneticField(grid);

    const bool electric_ok =
        E.x_center == Centering::CellCenter &&
        E.y_center == Centering::Node &&
        E.z_center == Centering::Node;

    const bool magnetic_ok =
        B.x_center == Centering::Node &&
        B.y_center == Centering::CellCenter &&
        B.z_center == Centering::CellCenter;

    return electric_ok && magnetic_ok;
}

bool testElectricGather() {
    const Grid grid{4, 1.0};

    VectorField E =
        makeElectricField(grid);

    // Ex is stored at cell centers.
    E.x = {10.0, 20.0, 30.0, 40.0};

    // Ey and Ez are stored at nodes.
    E.y = {0.0, 1.0, 2.0, 3.0};
    E.z = {0.0, 2.0, 4.0, 6.0};

    const Vector3 E_part =
        gatherField(E, grid, 0.25);

    // Ex uses centers 3 and 0.
    const bool x_ok =
        near(E_part.x, 17.5);

    // Ey and Ez use nodes 0 and 1.
    const bool y_ok =
        near(E_part.y, 0.25);

    const bool z_ok =
        near(E_part.z, 0.50);

    return x_ok && y_ok && z_ok;
}

bool testPeriodicGather() {
    const Grid grid{4, 1.0};

    VectorField B =
        makeMagneticField(grid);

    B.x = {0.0, 1.0, 2.0, 3.0};
    B.y = {10.0, 20.0, 30.0, 40.0};
    B.z = {0.0, 2.0, 4.0, 6.0};

    const Vector3 first =
        gatherField(B, grid, 0.25);

    const Vector3 wrapped =
        gatherField(B, grid, 4.25);

    return
        near(first.x, wrapped.x) &&
        near(first.y, wrapped.y) &&
        near(first.z, wrapped.z);
}

int main() {
    const bool layout_ok =
        testFieldLayout();

    const bool gather_ok =
        testElectricGather();

    const bool periodic_ok =
        testPeriodicGather();

    std::cout << "Yee field layout: "
              << (layout_ok ? "PASS" : "FAIL")
              << '\n';

    std::cout << "Field gather: "
              << (gather_ok ? "PASS" : "FAIL")
              << '\n';

    std::cout << "Periodic gather: "
              << (periodic_ok ? "PASS" : "FAIL")
              << '\n';

    const bool tests_ok =
        layout_ok &&
        gather_ok &&
        periodic_ok;

    return tests_ok ? 0 : 1;
}