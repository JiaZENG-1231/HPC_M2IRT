#include <cmath>
#include <iostream>

#include "Grid.hpp"

bool near(
    double a,
    double b,
    double tol = 1.0e-12
) {
    return std::abs(a - b) < tol;
}

bool testGridGeometry() {
    const Grid grid{4, 1.0};

    return
        grid.size() == 4 &&
        near(grid.spacing(), 1.0) &&
        near(grid.length(), 4.0) &&
        near(grid.nodePosition(2), 2.0) &&
        near(grid.cellCenterPosition(2), 2.5);
}

bool testPeriodicWrap() {
    const Grid grid{4, 1.0};

    return
        near(grid.wrap(0.25), 0.25) &&
        near(grid.wrap(4.25), 0.25) &&
        near(grid.wrap(8.25), 0.25) &&
        near(grid.wrap(-0.25), 3.75);
}

bool testNodeWeights() {
    const Grid grid{4, 1.0};

    const ShapeWeights inside =
        grid.weights(
            0.25,
            Centering::Node
        );

    const bool inside_ok =
        inside.left == 0 &&
        inside.right == 1 &&
        near(inside.w_left, 0.75) &&
        near(inside.w_right, 0.25);

    const ShapeWeights edge =
        grid.weights(
            3.75,
            Centering::Node
        );

    // Nodes 3 and 0 meet at the boundary.
    const bool edge_ok =
        edge.left == 3 &&
        edge.right == 0 &&
        near(edge.w_left, 0.25) &&
        near(edge.w_right, 0.75);

    return inside_ok && edge_ok;
}

bool testCenterWeights() {
    const Grid grid{4, 1.0};

    const ShapeWeights center =
        grid.weights(
            0.5,
            Centering::CellCenter
        );

    const bool center_ok =
        center.left == 0 &&
        center.right == 1 &&
        near(center.w_left, 1.0) &&
        near(center.w_right, 0.0);

    const ShapeWeights edge =
        grid.weights(
            0.25,
            Centering::CellCenter
        );

    const bool edge_ok =
        edge.left == 3 &&
        edge.right == 0 &&
        near(edge.w_left, 0.25) &&
        near(edge.w_right, 0.75);

    return center_ok && edge_ok;
}

int main() {
    const bool geometry_ok =
        testGridGeometry();

    const bool wrap_ok =
        testPeriodicWrap();

    const bool node_ok =
        testNodeWeights();

    const bool center_ok =
        testCenterWeights();

    std::cout << "Grid geometry: "
              << (geometry_ok ? "PASS" : "FAIL")
              << '\n';

    std::cout << "Periodic wrap: "
              << (wrap_ok ? "PASS" : "FAIL")
              << '\n';

    std::cout << "Node weights: "
              << (node_ok ? "PASS" : "FAIL")
              << '\n';

    std::cout << "Cell-center weights: "
              << (center_ok ? "PASS" : "FAIL")
              << '\n';

    const bool tests_ok =
        geometry_ok &&
        wrap_ok &&
        node_ok &&
        center_ok;

    return tests_ok ? 0 : 1;
}