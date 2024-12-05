#include "Sprite.h"

// Sprite
// ------

Sprite::Sprite(Color backgroundColor): backgroundColor(backgroundColor) {}

// Draws the sprite accorind to the provided coords/size in PIXELS.
void Sprite::draw(float x, float y, float width, float height) {}

// SRectangle
// ----------

SRectangle::SRectangle(Color backgroundColor): Sprite(backgroundColor) {}

void SRectangle::draw(float x, float y, float width, float height) {
    DrawRectangle(x, y, width, height, this->backgroundColor);   
}

// SEllipse
// ----------

SEllipse::SEllipse(Color backgroundColor): Sprite(backgroundColor) {}

void SEllipse::draw(float x, float y, float width, float height) {
    float radiusH = width / 2;
    float radiusV = height / 2;
    float centerX = x + radiusH;
    float centerY = y + radiusV;
    DrawEllipse(centerX, centerY, radiusH, radiusV, this->backgroundColor);
}

// SLine
// ----------

SLine::SLine(Color backgroundColor, float thickness): Sprite(backgroundColor),
    thickness(thickness) {}

void SLine::draw(float x, float y, float width, float height) {
    DrawLineEx({x, y}, {x + width, y + height}, this->thickness, this->backgroundColor);
}