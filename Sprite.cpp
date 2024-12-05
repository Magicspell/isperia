#include "Sprite.h"
#include <string>

using namespace std;

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

// SSquare
// ----------

SSquare::SSquare(Color backgroundColor): Sprite(backgroundColor) {}

void SSquare::draw(float x, float y, float width, float height) {
    DrawRectangle(x, y, width, width, this->backgroundColor);
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

// SCircle
// ----------

SCircle::SCircle(Color backgroundColor): Sprite(backgroundColor) {}

void SCircle::draw(float x, float y, float width, float height) {
    float radius = width / 2;
    float centerX = x + radius;
    float centerY = y + radius;
    DrawCircle(centerX, centerY, radius, this->backgroundColor);
}

// SLine
// ----------

SLine::SLine(Color backgroundColor, float thickness): Sprite(backgroundColor),
    thickness(thickness) {}

void SLine::draw(float x1, float y1, float x2, float y2) {
    DrawLineEx({x1, y1}, {x2, y2}, this->thickness, this->backgroundColor);
}

// SText
// ----------

SText::SText(Color backgroundColor, string text, int fontSize): Sprite(backgroundColor),
    font(GetFontDefault()), text(text), fontSize(fontSize) {}

SText::SText(Color backgroundColor, Font font, string text, int fontSize):
    Sprite(backgroundColor), font(font), text(text), fontSize(fontSize) {}

void SText::draw(float x, float y, float width, float height) {
    DrawTextEx(this->font, this->text.data(), {x, y}, this->fontSize, 0, this->backgroundColor);
}