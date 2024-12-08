// Common helper functions
#include "raylib.h"

bool pointInRect(float pX, float pY, float rX, float rY, float rWidth, float rHeight) {
    return (rX <= pX && pX <= (rX + rWidth)) && (rY <= pY && pY <= (rY + rHeight));
}

bool pointInRect(Vector2 point, Rectangle rect) {
    return pointInRect(point.x, point.y, rect.x, rect.y, rect.width, rect.height);
}