#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
#include "BorisPusher.hpp"
#include "FieldGrid.hpp"
#include "ParticleMesh.hpp"

// Compare two floating-point numbers within a given tolerance.
bool near(double a, double b, double tol = 1.0e-12) {
    return std::abs(a - b) < tol;
}

// Check the analytical solution for motion without fields.
bool testFreeMotion() {
    Particle p{
        {0.0, 0.0, 0.0},   // Initial position
        {1.0, 2.0, 0.0},   // Constant velocity
        1.0,
        1.0
    };

    const Vector3 E{0.0, 0.0, 0.0};
    const Vector3 B{0.0, 0.0, 0.0};
    const double dt = 0.1;

    borisPush(p, E, B, dt);

    const bool pos_ok =
        near(p.pos.x, 0.1) &&
        near(p.pos.y, 0.2) &&
        near(p.pos.z, 0.0);

    const bool vel_ok =
        near(p.vel.x, 1.0) &&
        near(p.vel.y, 2.0) &&
        near(p.vel.z, 0.0);

    return pos_ok && vel_ok;
}

// A magnetic field rotates the velocity but performs no work.
// Therefore, the speed must remain constant.
bool testMagneticEnergy() {
    Particle p{
        {0.0, 0.0, 0.0},
        {1.0, 0.0, 0.0},
        1.0,
        1.0
    };

    const Vector3 E{0.0, 0.0, 0.0};
    const Vector3 B{0.0, 0.0, 1.0};

    const double dt = 0.01;
    const int n_steps = 1000;

    const double speed_init = p.vel.norm();

    for (int step = 0; step < n_steps; ++step) {
        borisPush(p, E, B, dt);
    }

    const double speed_final = p.vel.norm();

    return near(speed_final, speed_init, 1.0e-10);
}

// Check the analytical solution for constant electric acceleration.
bool testElectricAcceleration() {
    const Vector3 E{1.0, 0.0, 0.0};
    const Vector3 B{0.0, 0.0, 0.0};

    const double dt = 0.01;
    const int n_steps = 100;
    const double q = 1.0;
    const double m = 1.0;

    // Acceleration produced by the constant electric field.
    const Vector3 acc = (q / m) * E;

    // The Boris scheme stores velocity at a half time step.
    const Vector3 vel_half = -0.5 * dt * acc;

    Particle p{
        {0.0, 0.0, 0.0},
        vel_half,
        q,
        m
    };

    for (int step = 0; step < n_steps; ++step) {
        borisPush(p, E, B, dt);
    }

    const double time = n_steps * dt;

    // Analytical position at the final integer time.
    const double x_exact = 0.5 * acc.x * time * time;

    // Stored velocity is at t - dt/2.
    const double v_exact = acc.x * (time - 0.5 * dt);

    const bool pos_ok = near(p.pos.x, x_exact, 1.0e-10);
    const bool vel_ok = near(p.vel.x, v_exact, 1.0e-10);

    return pos_ok && vel_ok;
}

bool testMultipleParticles() {
    // Particles with different initial positions and velocities.
    std::vector<Particle> particles{
        {{0.0, 0.0, 0.0}, { 1.0, 0.0, 0.0}, 1.0, 1.0},
        {{1.0, 2.0, 0.0}, { 0.0, 1.0, 0.0}, 1.0, 1.0},
        {{0.0, 0.0, 3.0}, {-1.0, 0.0, 0.0}, 1.0, 1.0}
    };

    // Without fields, each particle follows a straight line.
    const Vector3 E{0.0, 0.0, 0.0};
    const Vector3 B{0.0, 0.0, 0.0};
    const double dt = 0.1;

    borisPush(particles, E, B, dt);

    const bool first_ok =
        near(particles[0].pos.x, 0.1) &&
        near(particles[0].pos.y, 0.0);

    const bool second_ok =
        near(particles[1].pos.x, 1.0) &&
        near(particles[1].pos.y, 2.1);

    const bool third_ok =
        near(particles[2].pos.x, -0.1) &&
        near(particles[2].pos.z, 3.0);

    return first_ok && second_ok && third_ok;
}

bool testLocalFields() {
    // Two particles start from rest at different positions.
    std::vector<Particle> particles{
        {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, 1.0, 1.0},
        {{1.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, 1.0, 1.0}
    };

    // Each particle experiences a different electric field.
    const std::vector<Vector3> E_part{
        {1.0, 0.0, 0.0},
        {2.0, 0.0, 0.0}
    };

    // No magnetic field is applied.
    const std::vector<Vector3> B_part{
        {0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0}
    };

    const double dt = 0.1;

    borisPush(particles, E_part, B_part, dt);

    // The second particle should receive twice the velocity change.
    return near(particles[0].vel.x, 0.1) &&
           near(particles[1].vel.x, 0.2);
}

bool testFieldInterpolation() {
    // Grid points are located at x = 0, 1 and 2.
    // The electric field increases linearly with x.
    const std::vector<Vector3> E_grid{
        {0.0, 0.0, 0.0},
        {1.0, 0.0, 0.0},
        {2.0, 0.0, 0.0}
    };

    // A constant magnetic field is stored on the same grid.
    const std::vector<Vector3> B_grid{
        {0.0, 0.0, 1.0},
        {0.0, 0.0, 1.0},
        {0.0, 0.0, 1.0}
    };

    const FieldGrid grid{
        0.0, // Position of the first grid point
        1.0, // Grid spacing
        E_grid,
        B_grid
    };

    // At x = 0.25, linear interpolation should give E_x = 0.25.
    const Vector3 E_part = grid.electricField(0.25);
    const Vector3 B_part = grid.magneticField(0.25);

    const bool electric_ok =
        near(E_part.x, 0.25) &&
        near(E_part.y, 0.0) &&
        near(E_part.z, 0.0);

    const bool magnetic_ok =
        near(B_part.x, 0.0) &&
        near(B_part.y, 0.0) &&
        near(B_part.z, 1.0);

    return electric_ok && magnetic_ok;
}

bool testGridParticlePush() {
    // Electric field E_x = x on grid points x = 0, 1 and 2.
    const std::vector<Vector3> E_grid{
        {0.0, 0.0, 0.0},
        {1.0, 0.0, 0.0},
        {2.0, 0.0, 0.0}
    };

    // No magnetic field is applied.
    const std::vector<Vector3> B_grid{
        {0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0}
    };

    const FieldGrid grid{
        0.0, // First grid point
        1.0, // Grid spacing
        E_grid,
        B_grid
    };

    // Both particles start from rest at different positions.
    std::vector<Particle> particles{
        {{0.5, 0.0, 0.0}, {0.0, 0.0, 0.0}, 1.0, 1.0},
        {{1.5, 0.0, 0.0}, {0.0, 0.0, 0.0}, 1.0, 1.0}
    };

    const double dt = 0.1;

    // Fields are interpolated automatically before each particle is pushed.
    borisPush(particles, grid, dt);

    // At x = 0.5 and 1.5, the interpolated fields are 0.5 and 1.5.
    // Since q/m = 1, the expected velocity changes are E_x * dt.
    const bool first_ok =
        near(particles[0].vel.x, 0.05);

    const bool second_ok =
        near(particles[1].vel.x, 0.15);

    return first_ok && second_ok;
}

bool testChargeDeposition() {
    // A three-point grid located at x = 0, 1 and 2.
    const std::vector<Vector3> E_grid(3);
    const std::vector<Vector3> B_grid(3);

    const FieldGrid grid{
        0.0, // First grid point
        1.0, // Grid spacing
        E_grid,
        B_grid
    };

    // Both physical particle charges are q = 1.
    // The first macroparticle has weight 2 at x = 0.25.
    // The second macroparticle has weight 1 at x = 1.5.
    const std::vector<Particle> particles{
        {
            {0.25, 0.0, 0.0},
            {0.0, 0.0, 0.0},
            1.0, // q
            1.0, // m
            2.0  // w
        },
        {
            {1.50, 0.0, 0.0},
            {0.0, 0.0, 0.0},
            1.0, // q
            1.0, // m
            1.0  // w
        }
    };

    const std::vector<double> node_charge =
        depositCharge(particles, grid);

    // First particle:
    // node 0 receives 2 * 0.75 = 1.5
    // node 1 receives 2 * 0.25 = 0.5
    //
    // Second particle:
    // node 1 receives 1 * 0.5 = 0.5
    // node 2 receives 1 * 0.5 = 0.5
    const bool weights_ok =
        near(node_charge[0], 1.5) &&
        near(node_charge[1], 1.0) &&
        near(node_charge[2], 0.5);

    // Total grid charge must equal sum(q * w).
    const double total_charge =
        node_charge[0] +
        node_charge[1] +
        node_charge[2];

    const bool conservation_ok =
        near(total_charge, 3.0);

    return weights_ok && conservation_ok;
}

bool testGridMoments() {
    // Three grid points located at x = 0, 1 and 2.
    const std::vector<Vector3> E_grid(3);
    const std::vector<Vector3> B_grid(3);

    const FieldGrid grid{
        0.0,
        1.0,
        E_grid,
        B_grid
    };

    const std::vector<Particle> particles{
        {
            {0.25, 0.0, 0.0}, // Position
            {2.00, 0.0, 0.0}, // Velocity
            1.0,               // Charge
            1.0,               // Mass
            2.0                // Statistical weight
        },
        {
            {1.50, 0.0, 0.0}, // Position
            {0.00, 2.0, 0.0}, // Velocity
            1.0,               // Charge
            1.0,               // Mass
            1.0                // Statistical weight
        }
    };

    const GridMoments moments =
        computeMoments(particles, grid);

    // Expected density:
    //
    // Particle 0:
    // node 0 receives 2.0 * 0.75 = 1.5
    // node 1 receives 2.0 * 0.25 = 0.5
    //
    // Particle 1:
    // node 1 receives 1.0 * 0.50 = 0.5
    // node 2 receives 1.0 * 0.50 = 0.5
    const bool density_ok =
        near(moments.density[0], 1.5) &&
        near(moments.density[1], 1.0) &&
        near(moments.density[2], 0.5);

    // Flux is the deposited sum of weight times particle velocity.
    //
    // Node 0:
    // 1.5 * (2, 0, 0) = (3, 0, 0)
    //
    // Node 1:
    // 0.5 * (2, 0, 0) + 0.5 * (0, 2, 0)
    // = (1, 1, 0)
    //
    // Node 2:
    // 0.5 * (0, 2, 0) = (0, 1, 0)
    const bool flux_ok =
        near(moments.flux[0].x, 3.0) &&
        near(moments.flux[0].y, 0.0) &&
        near(moments.flux[0].z, 0.0) &&

        near(moments.flux[1].x, 1.0) &&
        near(moments.flux[1].y, 1.0) &&
        near(moments.flux[1].z, 0.0) &&

        near(moments.flux[2].x, 0.0) &&
        near(moments.flux[2].y, 1.0) &&
        near(moments.flux[2].z, 0.0);

    // Bulk velocity is calculated from velocity = flux / density.
    const bool velocity_ok =
        near(moments.velocity[0].x, 2.0) &&
        near(moments.velocity[0].y, 0.0) &&
        near(moments.velocity[0].z, 0.0) &&

        near(moments.velocity[1].x, 1.0) &&
        near(moments.velocity[1].y, 1.0) &&
        near(moments.velocity[1].z, 0.0) &&

        near(moments.velocity[2].x, 0.0) &&
        near(moments.velocity[2].y, 2.0) &&
        near(moments.velocity[2].z, 0.0);

    // The total deposited density must equal the sum of particle weights.
    double total_density = 0.0;

    for (const double value : moments.density) {
        total_density += value;
    }

    const bool conservation_ok =
        near(total_density, 3.0);

    return density_ok &&
           flux_ok &&
           velocity_ok &&
           conservation_ok;
}

bool testLarmorRadius() {
    // A charged particle moves perpendicular to a uniform magnetic field.
    Particle p{
        {0.0, 0.0, 0.0}, // Initial position
        {1.0, 0.0, 0.0}, // Initial perpendicular velocity
        1.0,              // Charge
        1.0               // Mass
    };

    const Vector3 E{0.0, 0.0, 0.0};
    const Vector3 B{0.0, 0.0, 1.0};

    const double dt = 0.005;
    const int n_steps = 1400;

    const double initial_speed = p.vel.norm();

    double x_min = p.pos.x;
    double x_max = p.pos.x;

    // Follow slightly more than one cyclotron period.
    for (int step = 0; step < n_steps; ++step) {
        borisPush(p, E, B, dt);

        x_min = std::min(x_min, p.pos.x);
        x_max = std::max(x_max, p.pos.x);
    }

    // The orbit diameter is the range of the x coordinate.
    const double radius_numerical =
        0.5 * (x_max - x_min);

    const double radius_exact =
        p.m * initial_speed /
        (std::abs(p.q) * B.norm());

    return near(
        radius_numerical,
        radius_exact,
        5.0e-3
    );
}

bool testCyclotronFrequency() {
    Particle p{
        {0.0, 0.0, 0.0},
        {1.0, 0.0, 0.0},
        1.0, // Charge
        1.0  // Mass
    };

    const Vector3 E{0.0, 0.0, 0.0};
    const Vector3 B{0.0, 0.0, 1.0};

    const double dt = 0.002;
    const int n_steps = 4000;
    const double pi = 3.14159265358979323846;

    double previous_vy = p.vel.y;
    double period_numerical = 0.0;

    bool positive_vy_reached = false;
    bool period_found = false;

    for (int step = 1; step <= n_steps; ++step) {
        borisPush(p, E, B, dt);

        const double current_vy = p.vel.y;

        // After half an orbit, vy becomes positive.
        if (current_vy > 0.0) {
            positive_vy_reached = true;
        }

        // One complete orbit is reached when vy changes
        // from positive back to negative.
        if (
            positive_vy_reached &&
            previous_vy > 0.0 &&
            current_vy <= 0.0
        ) {
            // Linear interpolation gives a more accurate
            // estimate of the zero crossing.
            const double fraction =
                previous_vy /
                (previous_vy - current_vy);

            period_numerical =
                (step - 1 + fraction) * dt;

            period_found = true;
            break;
        }

        previous_vy = current_vy;
    }

    if (!period_found) {
        return false;
    }

    const double frequency_numerical =
        2.0 * pi / period_numerical;

    const double frequency_exact =
        std::abs(p.q) * B.norm() / p.m;

    return near(
        frequency_numerical,
        frequency_exact,
        5.0e-3
    );
}

bool testRotationDirection() {
    Particle positive{
        {0.0, 0.0, 0.0},
        {1.0, 0.0, 0.0},
        1.0, // Positive charge
        1.0
    };

    Particle negative{
        {0.0, 0.0, 0.0},
        {1.0, 0.0, 0.0},
        -1.0, // Negative charge
        1.0
    };

    const Vector3 E{0.0, 0.0, 0.0};
    const Vector3 B{0.0, 0.0, 1.0};
    const double dt = 0.01;

    borisPush(positive, E, B, dt);
    borisPush(negative, E, B, dt);

    // A positive charge bends toward negative y.
    const bool positive_ok =
        positive.vel.y < 0.0 &&
        positive.pos.y < 0.0;

    // A negative charge bends toward positive y.
    const bool negative_ok =
        negative.vel.y > 0.0 &&
        negative.pos.y > 0.0;

    return positive_ok && negative_ok;
}

bool testExBCase(
    const Vector3& E,
    const Vector3& B,
    const Vector3& expected_drift
) {
    // Starting at the theoretical drift velocity removes
    // the cyclotron motion and leaves a straight trajectory.
    Particle p{
        {0.0, 0.0, 0.0},
        expected_drift,
        1.0,
        1.0
    };

    const double dt = 0.01;
    const int n_steps = 200;

    for (int step = 0; step < n_steps; ++step) {
        borisPush(p, E, B, dt);
    }

    const double time = n_steps * dt;
    const Vector3 expected_pos =
        time * expected_drift;

    const bool position_ok =
        near(p.pos.x, expected_pos.x, 1.0e-9) &&
        near(p.pos.y, expected_pos.y, 1.0e-9) &&
        near(p.pos.z, expected_pos.z, 1.0e-9);

    const bool velocity_ok =
        near(p.vel.x, expected_drift.x, 1.0e-9) &&
        near(p.vel.y, expected_drift.y, 1.0e-9) &&
        near(p.vel.z, expected_drift.z, 1.0e-9);

    return position_ok && velocity_ok;
}

bool testExBDrift() {
    // E along x and B along z gives drift along negative y.
    const bool xy_case = testExBCase(
        {1.0, 0.0, 0.0},
        {0.0, 0.0, 1.0},
        {0.0, -1.0, 0.0}
    );

    // E along y and B along x gives drift along negative z.
    const bool yz_case = testExBCase(
        {0.0, 1.0, 0.0},
        {1.0, 0.0, 0.0},
        {0.0, 0.0, -1.0}
    );

    // E along z and B along y gives drift along negative x.
    const bool zx_case = testExBCase(
        {0.0, 0.0, 1.0},
        {0.0, 1.0, 0.0},
        {-1.0, 0.0, 0.0}
    );

    return xy_case && yz_case && zx_case;
}

int main() {
    // Run all particle, field and grid tests.
    const bool free_motion_ok = testFreeMotion();
    const bool magnetic_energy_ok = testMagneticEnergy();
    const bool electric_acceleration_ok =
        testElectricAcceleration();

    const bool multiple_particles_ok =
        testMultipleParticles();

    const bool local_fields_ok = testLocalFields();
    const bool interpolation_ok = testFieldInterpolation();
    const bool grid_push_ok = testGridParticlePush();
    const bool deposition_ok = testChargeDeposition();
    const bool moments_ok = testGridMoments();
    const bool larmor_radius_ok = testLarmorRadius();
    const bool cyclotron_frequency_ok =
    testCyclotronFrequency();
    const bool rotation_direction_ok =
    testRotationDirection();
    const bool exb_drift_ok = testExBDrift();
    

    std::cout << "Free motion: "
              << (free_motion_ok ? "PASS" : "FAIL") << '\n';

    std::cout << "Magnetic energy conservation: "
              << (magnetic_energy_ok ? "PASS" : "FAIL") << '\n';

    std::cout << "Electric acceleration: "
              << (electric_acceleration_ok ? "PASS" : "FAIL")
              << '\n';

    std::cout << "Multiple particles: "
              << (multiple_particles_ok ? "PASS" : "FAIL")
              << '\n';

    std::cout << "Local particle fields: "
              << (local_fields_ok ? "PASS" : "FAIL") << '\n';

    std::cout << "Field interpolation: "
              << (interpolation_ok ? "PASS" : "FAIL") << '\n';

    std::cout << "Grid to particle push: "
              << (grid_push_ok ? "PASS" : "FAIL") << '\n';

    std::cout << "Particle charge deposition: "
              << (deposition_ok ? "PASS" : "FAIL") << '\n';

    std::cout << "Grid moments: "
              << (moments_ok ? "PASS" : "FAIL") << '\n';

    std::cout << "Larmor radius: "
              << (larmor_radius_ok ? "PASS" : "FAIL") << '\n';

    std::cout << "Cyclotron frequency: "
          << (cyclotron_frequency_ok ? "PASS" : "FAIL")
          << '\n';
    std::cout << "Rotation direction: "
          << (rotation_direction_ok ? "PASS" : "FAIL")
          << '\n';
    std::cout << "E cross B drift: "
          << (exb_drift_ok ? "PASS" : "FAIL")
          << '\n';
    // The test executable succeeds only if every test passes.

    const bool all_tests_ok =
        free_motion_ok &&
        magnetic_energy_ok &&
        electric_acceleration_ok &&
        multiple_particles_ok &&
        local_fields_ok &&
        interpolation_ok &&
        grid_push_ok &&
        deposition_ok &&
        moments_ok &&
        larmor_radius_ok &&
        cyclotron_frequency_ok &&
        rotation_direction_ok &&
        exb_drift_ok;

    return all_tests_ok ? 0 : 1;
}