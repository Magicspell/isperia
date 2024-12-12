// Common helper functions
#include <cmath>
#include "raylib.h"

bool pointInRect(float pX, float pY, float rX, float rY, float rWidth, float rHeight) {
    return (rX <= pX && pX <= (rX + rWidth)) && (rY <= pY && pY <= (rY + rHeight));
}

bool pointInRect(Vector2 point, Rectangle rect) {
    return pointInRect(point.x, point.y, rect.x, rect.y, rect.width, rect.height);
}

bool pointCloseToLine(float pX, float pY, float slope, float yInt, float buffer) {
    cout << "pX: " << pX << ", pY: " << pY << endl;
    cout << "SLOPE: " << slope << endl;
    cout << "yInt: " << yInt << endl;
    // cout << (slope * pX) + yInt - buffer << endl;
    // cout << (slope * pX) + yInt + buffer << endl;
    // return (pY > ((slope * pX) + yInt - buffer)) && (pY < ((slope * pX) + yInt + buffer));

    float a = slope;
    float b = -1;
    float c = yInt;

    float distance = abs(a * pX + b * pY + c) / sqrt(pow(a, 2) + pow(b, 2));

    cout << "DISTANCE: " << distance << endl;

    return distance < buffer;
}

bool pointCloseToLine(Vector2 point, float slope, float y, float buffer) {
    return pointCloseToLine(point.x, point.y, slope, y, buffer);
}