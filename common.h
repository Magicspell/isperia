// Common helper functions
#include "raylib.h"

bool point_in_rect(float pX, float pY, float rX, float rY, float rWidth, float rHeight) {
    return (rX <= pX && pX <= (rX + rWidth)) && (rY <= pY && pY <= (rY + rHeight));
}

bool point_in_rect(Vector2 point, Rectangle rect) {
    return point_in_rect(point.x, point.y, rect.x, rect.y, rect.width, rect.height);
}