#pragma once

#include <cmath>
#include <ostream>

// Three-dimensional vector used for position, velocity and fields.
struct Vector3 {
    double x = 0.0;   // x component
    double y = 0.0;   // y component
    double z = 0.0;   // z component

    Vector3() = default;

    // Initialize the three vector components.
    Vector3(double x_val, double y_val, double z_val)
        : x(x_val), y(y_val), z(z_val) {}

    // Vector addition.
    Vector3 operator+(const Vector3& rhs) const {
        return {x + rhs.x, y + rhs.y, z + rhs.z};
    }

    // Vector subtraction.
    Vector3 operator-(const Vector3& rhs) const {
        return {x - rhs.x, y - rhs.y, z - rhs.z};
    }

    // Multiplication by a scalar.
    Vector3 operator*(double s) const {
        return {x * s, y * s, z * s};
    }

    // Division by a scalar.
    Vector3 operator/(double s) const {
        return {x / s, y / s, z / s};
    }

    // Add another vector to the current vector.
    Vector3& operator+=(const Vector3& rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    // Squared magnitude, useful when the square root is unnecessary.
    double norm2() const {
        return x * x + y * y + z * z;
    }

    // Vector magnitude.
    double norm() const {
        return std::sqrt(norm2());
    }
};

// Allow scalar multiplication from the left: s * v.
inline Vector3 operator*(double s, const Vector3& v) {
    return v * s;
}

// Scalar product of two vectors.
inline double dot(const Vector3& a, const Vector3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// Cross product used in the magnetic rotation of the Boris pusher.
inline Vector3 cross(const Vector3& a, const Vector3& b) {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

// Print the three vector components.
inline std::ostream& operator<<(std::ostream& os, const Vector3& v) {
    os << v.x << ' ' << v.y << ' ' << v.z;
    return os;
}