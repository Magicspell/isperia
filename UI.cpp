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
            this->y += mouseDelta.y / pHeight;
            
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

void UIGraph::addVertex(float x, float y) {
    int id = this->backendGraph->getSize();
    this->vertices->push_back(new UIVertex(x, y, VERTEX_RADIUS, id));
    this->backendGraph->addVertex(x, y);
}

void UIGraph::addEdge(int v1, int v2) {
    // cout << "v1: " << v1 << ", v2: " << v2;
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

    // If the tool is ADD_VERTEX, then we need to check if user is clicking.
    // TODO: Code is copied from UIClickable, either make UIGraph a clickable or smth else.
    if (state.curTool == ADD_VERTEX && IsMouseButtonPressed(0) && pointInRect(GetMousePosition(), rect)) {
        Vector2 mousePos = GetMousePosition();

        // Convert mousePos to local coords TODO: helper
        float x = (mousePos.x - rect.x) / rect.width;
        float y = (mousePos.y - rect.y) / rect.height;

        this->addVertex(x - VERTEX_RADIUS / 2, y - VERTEX_RADIUS / 2);
    }


    for (UIEdge* e : *(this->edges)) {
        Rectangle edgeRect = e->update(rect.x, rect.y, rect.width, rect.height, state);


        // Check for deleting edges
        if (state.curTool == REMOVE_EDGE && IsMouseButtonPressed(0)) {
            // float slope = (-1) * (float) e->getY() / (float) e->getX();
            // float yInt = rect.height + rect.y - ((float) e->getY() * rect.width + rect.y);
            // Vector2 mousePos = GetMousePosition();
            // float pX = rect.width + rect.x - mousePos.x;
            // float pY = rect.height + rect.y - mousePos.y;

            // float globalX = e->getX() * rect.width + rect.x;
            // float globalY = e->getY() * rect.height  + rect.y;

            // float slope = (-1) * (float) globalY / (float) globalX;
            // float yInt = globalY - slope * globalX;
            Vector2 mousePos = GetMousePosition();
            float localMouseX = (mousePos.x - rect.x) / rect.width;
            float localMouseY = (rect.height - (mousePos.y - rect.y)) / rect.height;

            float x1 = e->getVertex1()->getX();
            float y1 = this->height - e->getVertex1()->getY();
            float x2 = e->getVertex2()->getX();
            float y2 = this->height - e->getVertex2()->getY();

            float slope = (y2 - y1) / (x2 - x1);
            float yInt = y1 - slope * x1;


            if (pointInRect(localMouseX, localMouseY, min(x1, x2), min(y1, y2), abs(x2 - x1), abs(y2 - y1)) 
                    && pointCloseToLine(localMouseX, localMouseY, slope, yInt, EDGE_DELETE_BUFFER)) {
                this->removeEdge(e);
            }
        }
    }

    for (UIVertex* v : *(this->vertices)) {
        Rectangle vertexRect = v->update(rect.x, rect.y, rect.width, rect.height, state);

        // If we are clicking on a vertex, proceed according to tool.
        if (IsMouseButtonPressed(0) && pointInRect(GetMousePosition(), vertexRect)) {
            switch (state.curTool) {
            case ADD_EDGE:
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
                break;
            case REMOVE_VERTEX:
                this->removeVertex(v);
            }
        }
    }

    return rect;
}

void UIGraph::removeVertex(UIVertex* vertex) {
    // Create new vertice vector
    vector<UIVertex*>* newVertices = new vector<UIVertex*>();

    for (UIVertex* v : *(this->vertices)) {
        if (v != vertex) {
            if (v->getId() > vertex->getId()) v->setId(v->getId() - 1);  // Ensure all ids match adj matrix.
            newVertices->push_back(v);
        }
    }

    // Remove underlying
    this->backendGraph->removeVertex(vertex->getId());

    vector<UIEdge*>* newEdges = new vector<UIEdge*>();
    // Remove edges
    for (UIEdge* e : *(this->edges)) {
        if (!(e->getVertex1() == vertex || e->getVertex2() == vertex)) {
            // ONLY remove UIEdge, since removeVertex() will also remove edge connections on the adj matrix.
            newEdges->push_back(e);
        }
    }

    delete this->edges;
    this->edges = newEdges;

    delete this->vertices;
    this->vertices = newVertices;
}

void UIGraph::removeEdge(UIEdge* e) {
    // Remove underlying
    this->backendGraph->removeEdge(e->getVertex1()->getId(), e->getVertex2()->getId());

    // Remove UIEdge
    this->edges->erase(find(this->edges->begin(), this->edges->end(), e));
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

// UIEigenProjGraph
// ----------------

UIEigenProjGraph::UIEigenProjGraph(float x, float y, float width, float height, Graph* backendGraph,
        vector<Sprite*>* sprites): UIGraph(x, y, width, height, backendGraph, sprites) {
    this->vertices = new vector<UIVertex*>();
    this->edges = new vector<UIEdge*>();
}

Rectangle UIEigenProjGraph::update(float pX, float pY, float pWidth, float pHeight, State state) {
    Rectangle rect = UIObject::update(pX, pY, pWidth, pHeight, state);

    // Check to rebuild the graph
    if ((this->vertices->size() != this->backendGraph->getSize())
            || (this->edges->size() != this->backendGraph->getEdgeCount())) {
        
        // Delete vertices
        for (UIVertex* v : *(this->vertices)) {
            delete v;
        }
        delete this->vertices;
        this->vertices = new vector<UIVertex*>();

        // Delete edges
        for (UIEdge* e : *(this->edges)) {
            delete e;
        }
        delete this->edges;
        this->edges = new vector<UIEdge*>();

        // Rebuild vertices
        float** coords = this->backendGraph->getEigenCoords();

        for (int i = 0; i < this->backendGraph->getSize(); i++) {
            // Normalize x, y from -1 to 1 to 0 to 1.
            float x = (coords[i][0] + 1) / 2;
            float y = (coords[i][1] + 1) / 2;

            this->vertices->push_back(new UIVertex(x, y, VERTEX_RADIUS, i));
        }

        // Rebuild edges
        for (int i = 0; i < this->backendGraph->getSize(); i++) {
            for (int j = 0; j < this->backendGraph->getSize(); j++) {
                if (this->backendGraph->getAdjMat()[i][j]) {
                    this->edges->push_back(new UIEdge(this->vertices->at(i), this->vertices->at(j)));
                }
            }
        }
    }

    // Normal drawing/updating
    for (UIEdge* e : *(this->edges)) {
        e->update(rect.x, rect.y, rect.width, rect.height);
    }
    for (UIVertex* v : *(this->vertices)) {
        v->update(rect.x, rect.y, rect.width, rect.height);
    }

    return rect;
}

void UIEigenProjGraph::draw(float x, float y, float width, float height, State state) {
    UIObject::draw(x, y, width, height, state);
}


// UIVertex
// --------

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

    this->textSprite->draw(x, y, width, width);     // TODO: THIS IS A JANK FIX TO ENSURE
                                                    // IT IS ALWAYS A SQUARE/CIRCLE,
                                                    // IMLEMENT AN ACTUAL WAY TO DO THIS!
}

Rectangle UIVertex::update(float pX, float pY, float pWidth, float pHeight, State state) {
    // Always make sure the sprite's text matches the ui object's text, since
    // we only change the ui object's.
    this->text = to_string(this->id);
    this->textSprite->setText(this->text);

    Rectangle rect;

    // If the tool is select, then we can move vertices, otherwise we cannot.
    if (state.curTool == SELECT) {
        rect =  UIDraggable::update(pX, pY, pWidth, pHeight, state);
    }
    rect =  UIObject::update(pX, pY, pWidth, pHeight, state);

    // cout << "rect.X: " << rect.x << ", rect.Y: " << rect.y << ", rect.W: " << rect.width << ", rect.H: " << rect.height << endl;
    // return rect;
    return {rect.x, rect.y, rect.width, rect.width};        // TODO: MORE CIRCLE/SQUARE JANK
}

int UIVertex::getId() {
    return this->id;
}

void UIVertex::setId(int id) {
    this->id = id;
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


    if (this->vertex1->getId() != this->vertex2->getId()) {
        // UIObject::draw(x, y, width, height, state);
        this->draw(x1, y1, x2, y2);
    } else {
        // Loop case
        float radius = VERTEX_RADIUS * pWidth / 2;
        DrawCircleLines(x1 + radius / 2, y1 + radius / 2, radius, WHITE);
    }

    return {x1, y1, x2 - x1, y2 - y1};  // TODO: Not actually the rectangle bc width/height
                                        // could be negative.
}

UIVertex* UIEdge::getVertex1() {
    return this->vertex1;
}

UIVertex* UIEdge::getVertex2() {
    return this->vertex2;
}

// UIToolbar
// ---------

// UIToolbar::UIToolbar(float x, float y, float width, float height):
//     UIToolbar(x, y, width, height, new vector<int>()) {}

UIToolbar::UIToolbar(float x, float y, float width, float height, vector<int>* tools,
        int curTool, vector<Sprite*>* sprites, vector<Sprite*>* toolSprites):
        UIObject(x, y, width, height, sprites), tools(tools), curTool(curTool), toolSprites(toolSprites) {
    // Set up tool buttons
    float paddingX = 0.1;
    float paddingY = 0.01;
    float buttonSize = 1.0 / this->tools->size();
    float curY = 0;
    int i = 0;
    for (int t : *(this->tools)) {
        Sprite* s;
        if (i >= this->toolSprites->size()) {
            s = new SText(BLACK, to_string(t), 20, CENTER);
        } else {
            s = this->toolSprites->at(i);
        }

        UIClickable* b = new UIClickable(paddingX, curY + paddingY, 1 - paddingX * 2,
                buttonSize - paddingY * 2, new vector<Sprite*>{
            new SRectangle(LIGHTGRAY),
            s
        }, [this, t](){
            this->setCurTool(t);
        });
        this->children->push_back(b);
        curY += buttonSize;
        i++;
    }
}

// UIToolbar::UIToolbar(float x, float y, float width, float height, vector<int>* tools, int curTool):
//     UIToolbar(x, y, width, height, tools, curTool, new vector<Sprite*>{ new SRectangle(GRAY) }) {}

void UIToolbar::setCurTool(int tool) {
    this->curTool = tool;
}

int UIToolbar::getCurTool() {
    return this->curTool;
}

// UIMatrix
// --------

UIMatrix::UIMatrix(float x, float y, float width, float height, Graph* graph, MatType matType):
    UIMatrix(x, y, width, height, graph, matType, new vector<Sprite*>()){}

UIMatrix::UIMatrix(float x, float y, float width, float height, Graph* graph, MatType matType, vector<Sprite*>* sprites):
        UIObject(x, y, width, height, sprites), graph(graph), matType(matType) {
    this->textSprites = new vector<SText*>(this->graph->getSize());
}

void UIMatrix::draw(float x, float y, float width, float height, State state) {
    UIObject::draw(x, y, width, height, state);

    // Draw text sprites over everything. We use one sprite and draw it multiple times. TODO: Use stack?
    SText* textSprite = new SText(MATRIX_TEXT_COLOR, "", 20, CENTER);

    float matrixPadding = MATRIX_PADDING * width;

    float incrementX = (width - (matrixPadding * 2)) / this->graph->getSize();
    float incrementY = (height - (matrixPadding * 2)) / this->graph->getSize();

    float curX = x + matrixPadding;
    float curY = y + matrixPadding;
    
    for (int i = 0; i < this->graph->getSize(); i++) {
        curX = x + matrixPadding;

        if (this->matType != EIG_VAL) {
            for (int j = 0; j < this->graph->getSize(); j++) {
                int roundNum = 1;
                if (this->matType == ADJ || this->matType == LAP) {
                    roundNum = 0;
                }

                string t = to_string_with_precision(this->graph->getMatByType(this->matType)[i][j], roundNum);

                if (t == "-0") t = "0"; // TODO: SCUFFED

                textSprite->setText(t);

                Vector2 textSize = MeasureTextEx(textSprite->getFont(), textSprite->getText().data(),
                    textSprite->getFontSize(), TEXT_SPACING);
                textSprite->draw(curX, curY + incrementY / 2, incrementX, 0);

                curX += incrementX;
            }
        } else {
            int roundNum = 1;
            if (this->matType == ADJ || this->matType == LAP) {
                roundNum = 0;
            }

            string t = to_string_with_precision(this->graph->getMatByType(this->matType)[i][0], roundNum);

            textSprite->setText(t);

            Vector2 textSize = MeasureTextEx(textSprite->getFont(), textSprite->getText().data(),
                textSprite->getFontSize(), TEXT_SPACING);
            textSprite->draw(curX, curY + incrementY / 2, incrementX, 0);
        }

        curY += incrementY;
    }
}

// UIInfoText
// -----

UIInfoText::UIInfoText(float x, float y, float width, float height, StringFuncType getNewText, string prefix,
    int fontSize, TextCentering textCentering): UIObject(x, y, width, height), getNewText(getNewText),
    prefix(prefix), fontSize(fontSize), textCentering(textCentering) {}

void UIInfoText::draw(float x, float y, float width, float height, State state) {
    UIObject::draw(x, y, width, height, state);

    // Get text string and draw it. TODO: Use stack?
    SText* textSprite = new SText(INFOTEXT_COLOR, this->prefix + this->getNewText(), this->fontSize,
        this->textCentering);
    textSprite->draw(x, y, width, height);
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