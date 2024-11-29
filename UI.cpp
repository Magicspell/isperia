#include "UI.h"

// UIObject
// --------

UIObject::UIObject(float x, float y, float width, float height, Color backgroundColor):
        x(x), y(y), width(width), height(height), backgroundColor(backgroundColor) {
    this->children = new vector<UIObject*>();
}

UIObject::~UIObject(){
    // TODO: Is this too slow? Or will we only ever delete at the end of the program so it
    // doesnt matter?
    for (UIObject* c : *(this->children)) {
        delete c;
    }
    delete this->children;
}

bool UIObject::getChanged() { return this->changed; }

// Draws an UIObject. Must call `_draw()` at somepoint in this function in order to
// draw children. Default is to draw children on top of background color. Takes the parent's
// coordinates as pixels.
void UIObject::draw(float pX, float pY, float pWidth, float pHeight) {
    float x = pX + pWidth * this->x;
    float y = pY + pHeight * this->y;
    float width = pWidth * this->width;
    float height = pHeight * this->height;

    // TODO: Optimize changed to mean "has this object changed since last frame OR have any
    // of its children"
    // if (this->changed) {
        DrawRectangle(x, y, width, height, this->backgroundColor);
        this->changed = false;
    // }
    _draw(x, y, width, height);
}

// Draws the children of a UIObject. Called from `draw()`. Takes the coordinates of the
// UIObject (not it's parent, this is different from `draw()`) in pixels.
void UIObject::_draw(float x, float y, float width, float height) { 
    for (UIObject* c : *(this->children)) {
        c->draw(x, y, width, height);
    }
}

// UIApp
// -----

UIApp::UIApp(UIObject* curScreen): curScreen(curScreen) {};

UIApp::~UIApp() {
    delete this->curScreen;
}

void UIApp::update() {
    BeginDrawing();
    // if (this->curScreen->getChanged()) {
    this->curScreen->draw(0, 0, GetScreenWidth(), GetScreenHeight());
    // }
    EndDrawing();
}