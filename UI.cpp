#include "UI.h"
#include "common.h"

using namespace std;

// UIObject
// --------

UIObject::UIObject(float x, float y, float width, float height, Sprite* sprite,
    vector<UIObject*>* children): x(x), y(y), width(width), height(height), sprite(sprite),
    children(children) {}

UIObject::UIObject(float x, float y, float width, float height, Sprite* sprite):
        x(x), y(y), width(width), height(height), sprite(sprite),
        children(new vector<UIObject*>) {}

UIObject::~UIObject(){
    // TODO: Is this too slow? Or will we only ever delete at the end of the program so it
    // doesnt matter?
    for (UIObject* c : *(this->children)) {
        delete c;
    }
    delete this->children;
    delete this->sprite;
}

bool UIObject::getChanged() { return this->changed; }

// Draws an UIObject. Parameters are the coords/size of ITSELF in PIXELS.
void UIObject::draw(float x, float y, float width, float height) {
    // DrawRectangle(x, y, width, height, this->backgroundColor);
    this->sprite->draw(x, y, width, height);
}

// Updates all children. Parameters are the coords/size of its PARENT in PIXELS.
// Will call `draw()`. Returns the coords/size of ITSELF in PIXELS as a rectangle.
Rectangle UIObject::update(float pX, float pY, float pWidth, float pHeight) {
    float x = pX + pWidth * this->x;
    float y = pY + pHeight * this->y;
    float width = pWidth * this->width;
    float height = pHeight * this->height;

    // TODO: Optimize changed to mean "has this object changed since last frame OR have any
    // of its children"
    // TODO: We can't implement changed without the screen flashing black every other frame.
    // if (this->changed) {
        this->draw(x, y, width, height);
        // this->changed = false;
    // }

    for (UIObject* c : *(this->children)) {
        c->update(x, y, width, height);
    }

    return {x, y, width, height};
}

void UIObject::setChildren(vector<UIObject*>* children) {
    this->children = children;
}

vector<UIObject*>* UIObject::getChildren(vector<UIObject*>* children) {
    return this->children;
}


// UIClickable
// -----------

UIClickable::UIClickable(float x, float y, float width, float height, Sprite* sprite,
    FuncType click): UIObject(x, y, width, height, sprite), click(click) {}

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

UIDraggable::UIDraggable(float x, float y, float width, float height, Sprite* sprite,
    FuncType click): UIClickable(x, y, width, height, sprite, click) {}

Rectangle UIDraggable::update(float pX, float pY, float pWidth, float pHeight) {
    if (this->isClicked) {
        // If we are being dragged, move according to mouse delta.
            Vector2 mouseDelta = GetMouseDelta();

            // We have to convert to percentage of parent. TODO: Helper function.
            this->x += mouseDelta.x / pWidth;
            this->y += mouseDelta.y / pWidth;
            
            // Ensure we are still in parent bounds. TODO: Do this by middle, not top left.
            if (this->x > 1) this->x = 1;
            if (this->x < 0) this->x = 0;
            if (this->y > 1) this->y = 1;
            if (this->y < 0) this->y = 0;
            this->changed = true;
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
    this->curScreen->update(0, 0, GetScreenWidth(), GetScreenHeight());
    EndDrawing();
}