#include "UI.h"
#include "common.h"

using namespace std;

// UIObject
// --------

UIObject::UIObject(float x, float y, float width, float height): UIObject(x, y, width, height, new vector<Sprite*>()) {}

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
void UIObject::draw(float x, float y, float width, float height, State state) {
    for (Sprite* s : *this->sprites) {
        s->draw(x, y, width, height);
    }
}

// Updates all children. Parameters are the coords/size of its PARENT in PIXELS.
// Will call `draw()`. Returns the coords/size of ITSELF in PIXELS as a rectangle.
Rectangle UIObject::update(float pX, float pY, float pWidth, float pHeight, State state) {
    float x = pX + pWidth * this->x;
    float y = pY + pHeight * this->y;
    float width = pWidth * this->width;
    float height = pHeight * this->height;

    // TODO: Optimize changed to mean "has this object changed since last frame OR have any
    // of its children"
    // TODO: We can't implement changed without the screen flashing black every other frame.
    // if (this->changed) {
        this->draw(x, y, width, height, state);
        // this->changed = false;
    // }

    for (UIObject* c : *(this->children)) {
        c->update(x, y, width, height, state);
    }

    return {x, y, width, height};
}

void UIObject::setChildren(vector<UIObject*>* children) {
    this->children = children;
}

void UIObject::addChild(UIObject* child) {
    this->children->push_back(child);
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

Rectangle UIClickable::update(float pX, float pY, float pWidth, float pHeight, State state) {
    Rectangle rect = UIObject::update(pX, pY, pWidth, pHeight, state);

    // Check to see if the user is clicking the button.
    if (IsMouseButtonPressed(0)) {
        if (pointInRect(GetMousePosition(), rect)) {
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

Rectangle UIDraggable::update(float pX, float pY, float pWidth, float pHeight, State state) {
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

    return UIClickable::update(pX, pY, pWidth, pHeight, state);
}

// UIGraph
// -------

UIGraph::UIGraph(float x, float y, float width, float height, Graph* backendGraph, vector<Sprite*>* sprites):
        UIObject(x, y, width, height, sprites), backendGraph(backendGraph) {
    this->vertices = new vector<UIVertex*>();
    this->edges = new vector<UIEdge*>();
}

// UIGraph::UIGraph(float x, float y, float width, float height):
//     UIGraph(x, y, width, height, new Graph(), new vector<Sprite*>{ new SRectangle(BLACK) }) {}
//     // TODO: Default colors


// void UIGraph::addVertex(UIVertex* vertex) {
//     this->vertices->push_back(vertex);
// }

// void UIGraph::addEdge(UIEdge* edge) {
//     this->edges->push_back(edge);
// }

void UIGraph::addVertex(float x, float y) {
    int id = this->backendGraph->getSize();
    this->vertices->push_back(new UIVertex(x, y, 0.1, id));
    this->backendGraph->addVertex(x, y);
}

void UIGraph::addEdge(int v1, int v2) {
    this->edges->push_back(new UIEdge(this->vertices->at(v1), this->vertices->at(v2)));
    this->backendGraph->addEdge(v1, v2);
}

vector<UIVertex*>* UIGraph::getVertices() {
    return this->vertices;
}

vector<UIEdge*>* UIGraph::getEdges() {
    return this->edges;
}

void UIGraph::setVertices(vector<UIVertex*>* vertices) {
    this->vertices = vertices;
}

void UIGraph::setEdges(vector<UIEdge*>* edges) {
    this->edges = edges;
}

Rectangle UIGraph::update(float pX, float pY, float pWidth, float pHeight, State state) {
    Rectangle rect = UIObject::update(pX, pY, pWidth, pHeight, state);

    if (state.curTool != ADD_EDGE || IsMouseButtonPressed(1)) this->edgeAddMode = NONE_SELECTED;

    // if (IsMouseButtonPressed(1)) this->edgeAddMode = NONE_SELECTED;

    // If the tool is ADD_VERTEX, then we need to check if user is clicking.
    // TODO: Code is copied from UIClickable, either make UIGraph a clickable or smth else.
    if (state.curTool == ADD_VERTEX && IsMouseButtonPressed(0) && pointInRect(GetMousePosition(), rect)) {
        Vector2 mousePos = GetMousePosition();

        // Convert mousePos to local coords TODO: helper
        float x = (mousePos.x - pX) / pWidth;
        float y = (mousePos.y - pY) / pHeight;

        this->addVertex(x, y);
    }

    for (UIEdge* e : *(this->edges)) {
        e->update(rect.x, rect.y, rect.width, rect.height, state);
    }

    for (UIVertex* v : *(this->vertices)) {
        Rectangle vrect = v->update(rect.x, rect.y, rect.width, rect.height, state);

        // Check if we are in edge add mode and if so check if we clicked on a vertex.
        if (state.curTool == ADD_EDGE && IsMouseButtonPressed(0) && pointInRect(GetMousePosition(), vrect)) {
            // cout << "CLICKED VERTEX: " << (int) v << endl;
            switch (this->edgeAddMode) {
            case NONE_SELECTED:
                this->v1 = v;
                this->edgeAddMode = ONE_SELECTED;
                break;
            case ONE_SELECTED:
                this->addEdge(this->v1->getId(), v->getId());
                this->edgeAddMode = NONE_SELECTED;
                break;
            }
        }
    }

    return rect;
}

void UIGraph::draw(float x, float y, float width, float height, State state) {
    UIObject::draw(x, y, width, height, state);

    // Draw line if we are adding an edge
    if (state.curTool == ADD_EDGE && this->edgeAddMode == ONE_SELECTED) {
        float vX = this->v1->getX() * width + x + (this->v1->getWidth() * width / 2);
        float vY = this->v1->getY() * height + y + (this->v1->getHeight() * height / 2);

        DrawLineEx({vX, vY}, GetMousePosition(), 1 /* TODO: change to default var */, WHITE /* TODO: SAME*/);
    }
}

Graph* UIGraph::getBackendGraph() {
    return this->backendGraph;
}

// UIVertex
// --------

// UIVertex::UIVertex(float x, float y, float width, float height, vector<Sprite*>* sprites):
//     UIDraggable(x, y, width, height, sprites) {}

// UIVertex::UIVertex(float x, float y, float width, float height):
//     UIDraggable(x, y, width, height, new vector<Sprite*>{ new SCircle(RED) }) {}
//     // TODO: Default colors

UIVertex::UIVertex(float x, float y, float radius, int id):
    UIVertex(x, y, radius, id, to_string(id)) {}

UIVertex::UIVertex(float x, float y, float radius, int id, string text, Color textColor, int fontSize):
    UIDraggable(x, y, radius, radius, new vector<Sprite*>{new SCircle(RED)}), id(id), text(text) {
        // We keep a seperate sprite outside of sprites for text that is aways drawn over
        // everything.
        this->textSprite = new SText(textColor, text, fontSize, CENTER);
}

void UIVertex::draw(float x, float y, float width, float height, State state) {
    UIDraggable::draw(x, y, width, height, state);

    this->textSprite->draw(x, y, width, height);
}

Rectangle UIVertex::update(float pX, float pY, float pWidth, float pHeight, State state) {
    // Always make sure the sprite's text matches the ui object's text, since
    // we only change the ui object's.
    this->textSprite->setText(this->text);

    // If the tool is select, then we can move vertices, otherwise we cannot.
    if (state.curTool == SELECT) {
        return UIDraggable::update(pX, pY, pWidth, pHeight, state);
    }
    return UIObject::update(pX, pY, pWidth, pHeight, state);
}

int UIVertex::getId() {
    return this->id;
}

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

Rectangle UIEdge::update(float pX, float pY, float pWidth, float pHeight, State state) {
    // Find pixel values from vertex coords.
    float x1 = pX + pWidth  * ((this->vertex1->getWidth()   / 2) + this->vertex1->getX());
    float y1 = pY + pHeight * ((this->vertex1->getHeight()  / 2) + this->vertex1->getY());
    float x2 = pX + pWidth  * ((this->vertex2->getWidth()   / 2) + this->vertex2->getX());
    float y2 = pY + pHeight * ((this->vertex2->getHeight()  / 2) + this->vertex2->getY());

    // Draw sprite. NOTE/TODO: this will break when the sprite isnt an SLine, since we are
    // not using width/height. Also, TODO, weird because Sprite::draw args are x, y, width,
    // and height and SLine::draw is x1, y1, x2, y2, so the arg locations mean different things.

    this->draw(x1, y1, x2, y2);

    return {x1, y1, x2 - x1, y2 - y1};  // TODO: Not actually the rectangle bc width/height
                                        // could be negative.
}

// UIToolbar
// ---------

UIToolbar::UIToolbar(float x, float y, float width, float height):
    UIToolbar(x, y, width, height, new vector<int>()) {}

UIToolbar::UIToolbar(float x, float y, float width, float height, vector<int>* tools, int curTool):
        UIObject(x, y, width, height, new vector<Sprite*>{ new SRectangle(GRAY) }), tools(tools),
        curTool(curTool) {
    
    // Set up tool buttons
    float paddingX = 0.1;
    float paddingY = 0.01;
    float buttonSize = 1.0 / this->tools->size();
    cout << buttonSize << endl;
    float curY = 0;
    for (int t : *(this->tools)) {
        UIClickable* b = new UIClickable(paddingX, curY + paddingY, 1 - paddingX * 2,
                buttonSize - paddingY * 2, new vector<Sprite*>{
            new SRectangle(LIGHTGRAY),
            new SText(BLACK, to_string(t), 20, CENTER)
        }, [this, t](){
            this->setCurTool(t);
        });
        this->children->push_back(b);
        curY += buttonSize;
    }
}

void UIToolbar::setCurTool(int tool) {
    this->curTool = tool;
}

int UIToolbar::getCurTool() {
    return this->curTool;
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