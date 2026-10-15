#pragma once

#include <cmath>
#include <ostream>

// Three-dimensional vector used for positions, velocities and fields.
struct Vector3 {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    Vector3& operator+=(const Vector3& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vector3& operator-=(const Vector3& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    // Squared magnitude avoids an unnecessary square root when possible.
    double norm2() const {
        return x * x + y * y + z * z;
    }

    double norm() const {
        return std::sqrt(norm2());
    }
};

inline Vector3 operator+(
    Vector3 a,
    const Vector3& b
) {
    a += b;
    return a;
}

inline Vector3 operator-(
    Vector3 a,
    const Vector3& b
) {
    a -= b;
    return a;
}

inline Vector3 operator*(
    const Vector3& v,
    double s
) {
    return {
        v.x * s,
        v.y * s,
        v.z * s
    };
}

inline Vector3 operator*(
    double s,
    const Vector3& v
) {
    return v * s;
}

inline Vector3 operator/(
    const Vector3& v,
    double s
) {
    return {
        v.x / s,
        v.y / s,
        v.z / s
    };
}

inline double dot(
    const Vector3& a,
    const Vector3& b
) {
    return a.x * b.x +
           a.y * b.y +
           a.z * b.z;
}

inline Vector3 cross(
    const Vector3& a,
    const Vector3& b
) {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

inline std::ostream& operator<<(
    std::ostream& out,
    const Vector3& v
) {
    out << v.x << ' '
        << v.y << ' '
        << v.z;

    return out;
}
