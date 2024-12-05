#include "UI.h"
#include "common.h"

using namespace std;

// UIObject
// --------

UIObject::UIObject(float x, float y, float width, float height, vector<Sprite*>* sprites,
    vector<UIObject*>* children): x(x), y(y), width(width), height(height), sprites(sprites),
    children(children) {}

UIObject::UIObject(float x, float y, float width, float height, vector<Sprite*>* sprites):
        x(x), y(y), width(width), height(height), sprites(sprites),
        children(new vector<UIObject*>) {}

UIObject::~UIObject(){
    // TODO: Is this too slow? Or will we only ever delete at the end of the program so it
    // doesnt matter?
    // TODO: We do not delete sprites as there could be other objects referencing them.
    // Should we?
    for (UIObject* c : *(this->children)) {
        delete c;
    }
    delete this->children;
}

bool UIObject::getChanged() { return this->changed; }

// Draws an UIObject. Parameters are the coords/size of ITSELF in PIXELS.
void UIObject::draw(float x, float y, float width, float height) {
    for (Sprite* s : *this->sprites) {
        s->draw(x, y, width, height);
    }
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

float UIObject::getX() {
    return this->x;
}

float UIObject::getY() {
    return this->y;
}

float UIObject::getWidth() {
    return this->width;
}

float UIObject::getHeight() {
    return this->height;
}

void UIObject::setX(float x) {
    this->x = x;
}

void UIObject::setY(float y) {
    this->y = y;
}

void UIObject::setWidth(float width) {
    this->width = width;
}

void UIObject::setHeight(float height) {
    this->height = height;
}

void UIObject::addSprite(Sprite* sprite) {
    this->sprites->push_back(sprite);
}


// UIClickable
// -----------

UIClickable::UIClickable(float x, float y, float width, float height, vector<Sprite*>* sprites,
    FuncType click): UIObject(x, y, width, height, sprites), click(click) {}

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

UIDraggable::UIDraggable(float x, float y, float width, float height, vector<Sprite*>* sprites,
    FuncType click): UIClickable(x, y, width, height, sprites, click) {}

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

// UIGraph
// -------

UIGraph::UIGraph(float x, float y, float width, float height, vector<Sprite*>* sprites):
    UIObject(x, y, width, height, sprites) {}

UIGraph::UIGraph(float x, float y, float width, float height):
    UIObject(x, y, width, height, new vector<Sprite*>{ new SRectangle(BLACK) }) {}
    // TODO: Default colors

// UIVertex
// --------

UIVertex::UIVertex(float x, float y, float width, float height, vector<Sprite*>* sprites):
    UIDraggable(x, y, width, height, sprites) {}

UIVertex::UIVertex(float x, float y, float width, float height):
    UIDraggable(x, y, width, height, new vector<Sprite*>{ new SCircle(RED) }) {}
    // TODO: Default colors

UIVertex::UIVertex(float x, float y, float radius, string text, Color textColor, int fontSize):
    UIDraggable(x, y, radius, radius, new vector<Sprite*>{
        new SCircle(RED),
        new SText(textColor, text, fontSize)
    }), text(text) {}

// UIEdge
// --------

// Edges must be siblings with their corresponding vertices as the coordinates are local
// (percentages).

UIEdge::UIEdge(UIVertex* vertex1, UIVertex* vertex2):
    UIObject(vertex1->getX(), vertex1->getY(), vertex2->getX() - vertex1->getX(),
    vertex2->getY() - vertex1->getY(), new vector<Sprite*>{ new SLine(WHITE) }), vertex1(vertex1), vertex2(vertex2) {}
    // TODO: Default colors

UIEdge::UIEdge(UIVertex* vertex1, UIVertex* vertex2, vector<Sprite*>* sprites):
    UIObject(vertex1->getX(), vertex1->getY(), vertex2->getX() - vertex1->getX(),
    vertex2->getY() - vertex1->getY(), sprites), vertex1(vertex1), vertex2(vertex2) {}

Rectangle UIEdge::update(float pX, float pY, float pWidth, float pHeight) {
    // Find pixel values from vertex coords.
    float x1 = pX + pWidth  * ((this->vertex1->getWidth() / 2)  + this->vertex1->getX());
    float y1 = pY + pHeight * ((this->vertex1->getHeight() / 2) + this->vertex1->getY());
    float x2 = pX + pWidth  * ((this->vertex2->getWidth() / 2)  + this->vertex2->getX());
    float y2 = pY + pHeight * ((this->vertex2->getHeight() / 2) + this->vertex2->getY());

    // Draw sprite. NOTE/TODO: this will break when the sprite isnt an SLine, since we are
    // not using width/height. Also, TODO, weird because Sprite::draw args are x, y, width,
    // and height and SLine::draw is x1, y1, x2, y2, so the arg locations mean different things.

    this->draw(x1, y1, x2, y2);

    return {x1, y1, x2 - x1, y2 - y1};  // TODO: Not actually the rectangle bc width/height
                                        // could be negative.
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