#ifndef MATHHELPER_H
#define MATHHELPER_H
#include <math.h>

#define PI 3.14159265
#define DEG_TO_RAD PI / 180.0f

struct Vector2 {

    double x;
    double y;

    Vector2(double _x = 0.0f, float _y = 0.0f)
        : x(_x), y(_y) {}

    double sqrMagnitude() {
        return x * x + y * y;
    }

    double magnitude() {
        return sqrt(sqrMagnitude());
    }

    Vector2 normalized() {
        float length = magnitude();
        return Vector2(x / length, y / length);
    }

    Vector2 operator+(const Vector2& rhs)
    {
        return Vector2(x + rhs.x, y + rhs.y);
    }

    Vector2 operator-(const Vector2& rhs)
    {
        return Vector2(x - rhs.x, y - rhs.y);
    }

    Vector2 operator*(const Vector2& rhs)
    {
        return Vector2(x * rhs.x, y * rhs.y);
    }

    Vector2 operator/(const double& rhs)
    {
        return Vector2(x / rhs, y / rhs);
    }

    Vector2 operator*(const double& rhs)
    {
        return Vector2(x * rhs, y * rhs);
    }

    double dotProduct(const Vector2& rhs)
    {
        return x * rhs.x + y * rhs.y;
    }
};

extern Vector2 RotateVector(Vector2& vec, float angle);

#endif
