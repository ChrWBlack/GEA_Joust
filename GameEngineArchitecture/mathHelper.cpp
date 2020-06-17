#include "mathHelper.h"

Vector2 RotateVector(Vector2& vec, float angle) {
    float radAngle = (float)(angle*DEG_TO_RAD);
    return Vector2(vec.x * cos(radAngle) - vec.y * sin(radAngle), vec.x * sin(radAngle) + vec.y * cos(radAngle));
}