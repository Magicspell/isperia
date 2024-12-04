#include <iostream>
#include "UI.h"
#include "common.h"

using namespace std;

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

// Draws an UIObject. Parameters are the coords/size of ITSELF in PIXELS.
void UIObject::draw(float x, float y, float width, float height) {

    // TODO: Optimize changed to mean "has this object changed since last frame OR have any
    // of its children"
    // if (this->changed) {
        DrawRectangle(x, y, width, height, this->backgroundColor);
        // this->changed = false;
    // }
    // _draw(x, y, width, height);
}

// Updates all children. Parameters are the coords/size of its PARENT in PIXELS.
// Will call `draw()`. Returns the coords/size of ITSELF in PIXELS as a rectangle.
Rectangle UIObject::update(float pX, float pY, float pWidth, float pHeight) {
    float x = pX + pWidth * this->x;
    float y = pY + pHeight * this->y;
    float width = pWidth * this->width;
    float height = pHeight * this->height;

    this->draw(x, y, width, height);

    for (UIObject* c : *(this->children)) {
        c->update(x, y, width, height);
    }

    return {x, y, width, height};
}

// UIClickable
// -----------

UIClickable::UIClickable(float x, float y, float width, float height, FuncType click,
    Color backgroundColor): UIObject(x, y, width, height, backgroundColor), click(click) {}

void UIClickable::activateClick() {
    this->click();
}

Rectangle UIClickable::update(float pX, float pY, float pWidth, float pHeight) {
    Rectangle rect = UIObject::update(pX, pY, pWidth, pHeight);

    // Check to see if the user is clicking the button.
    if (IsMouseButtonPressed(0)) {
        if (point_in_rect(GetMousePosition(), rect)) {
            this->isClicked = true;
            this->activateClick();
        }
    }
    
    if (this->isClicked && !IsMouseButtonDown(0)) {
        this->isClicked = false;
        this->releaseClick();
    }

    return rect;
}

void UIClickable::releaseClick() {
    // Placeholder
}

// UIDraggable
// -----------

UIDraggable::UIDraggable(float x, float y, float width, float height, FuncType click,
    Color backgroundColor): UIClickable(x, y, width, height, click, backgroundColor) {}

Rectangle UIDraggable::update(float pX, float pY, float pWidth, float pHeight) {
    if (this->isClicked) {
        // If we are being dragged, move according to mouse delta.
            Vector2 mouseDelta = GetMouseDelta();

            // We have to convert to percentage of parent. TODO: Helper function.
            this->x += (mouseDelta.x - pX) / pWidth;
            this->y += (mouseDelta.y - pY) / pWidth;
            
            // Ensure we are still in parent bounds. TODO: Do this by middle, not top left.
            if (this->x > 1) this->x = 1;
            if (this->x < 0) this->x = 0;
            if (this->y > 1) this->y = 1;
            if (this->y < 0) this->y = 0;

    }

    return UIClickable::update(pX, pY, pWidth, pHeight);
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
    this->curScreen->update(0, 0, GetScreenWidth(), GetScreenHeight());
    // }
    EndDrawing();
}