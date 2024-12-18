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

SText::SText(Color backgroundColor, string text, int fontSize, TextCentering textCentering):
    Sprite(backgroundColor), font(GetFontDefault()), text(text), fontSize(fontSize),
    textCentering(textCentering) {}

SText::SText(Color backgroundColor, Font font, string text, int fontSize,
    TextCentering textCentering): Sprite(backgroundColor), font(font), text(text),
    fontSize(fontSize), textCentering(textCentering) {}

void SText::draw(float x, float y, float width, float height) {
    Vector2 textSize = MeasureTextEx(this->font, this->text.data(), this->fontSize, 1);
    y = y + height / 2 - textSize.y / 2;    // Text is always centered vertically.

    switch (this->textCentering) {
    case LEFT: break;
    case CENTER:
        x = x + width / 2 - textSize.x / 2;
        break;
    case RIGHT:
        x = x + width - textSize.x;
        break;
    }
    DrawTextEx(this->font, this->text.data(), {x, y}, this->fontSize, TEXT_SPACING, this->backgroundColor);
}

void SText::setText(string text) {
    this->text = text;
}

string SText::getText() {
    return this->text;
}

Font SText::getFont(){
    return this->font;    
}

void SText::setFont(Font font){
    this->font = font;
}

int SText::getFontSize(){
    return this->fontSize;    
}

void SText::setFontSize(int fontSize){
    this->fontSize = fontSize;
}

SVertex::SVertex(Color backgroundColor): SCircle(backgroundColor) {}

void SVertex::draw(float x, float y, float width, float height) {
    // Center circle and pad it
    float padding = width * SVERTEX_PADDING;
    float radius = width / 2 - padding * 2;
    float centerX = x + width / 2 + padding;
    float centerY = y + height / 2 + padding;
    DrawCircle(centerX, centerY, radius, this->backgroundColor);
}

SCursor::SCursor(Color backgroundColor): Sprite(backgroundColor) {}

void SCursor::draw(float x, float y, float width, float height) {
    // Draw triangle from 0,0 -> 1,0.5 -> 0.5,1

    float padding = width * SCURSOR_PADDING;

    float x1 = x + padding;
    float y1 = y + padding;
    float x2 = x + width - padding;
    float y2 = y + height / 2 - padding;
    float x3 = x + width / 2 - padding;
    float y3 = y + height - padding;

    DrawTriangle({x3, y3}, {x2, y2}, {x1, y1}, this->backgroundColor);

    // Draw line from 0.5,0.5 -> 1,1
    DrawLineEx({x3, y2}, {x2, y3}, width * 0.1, this->backgroundColor);

}