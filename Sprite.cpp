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