#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include "raylib.h"
#include "Sprite.h"
#include "Graph.h"

#define DEFAULT_COLOR { 200, 125, 230, 255 }
#define DEFAULT_CLICK [](){}    // Lambda that does nothing

struct State {
    int curTool;
};

enum Tool {
    SELECT,
    ADD_VERTEX
};

#define DEFAULT_STATE {0}

using namespace std;

class UIObject {
public:
    UIObject(float x, float y, float width, float height);
    UIObject(float x, float y, float width, float height, vector<Sprite*>* sprites);
    UIObject(float x, float y, float width, float height, vector<Sprite*>* sprites,
        vector<UIObject*>* children);
    ~UIObject();
    virtual void draw(float x, float y, float width, float height);
    virtual Rectangle update(float pX, float pY, float pWidth, float pHeight, State state = DEFAULT_STATE);
    bool getChanged();
    void setChildren(vector<UIObject*>* children);
    void addChild(UIObject* child);
    vector<UIObject*>* getChildren(vector<UIObject*>* children);
    float getX();
    float getY();
    float getWidth();
    float getHeight();
    void setX(float x);
    void setY(float y);
    void setWidth(float width);
    void setHeight(float height);
    void addSprite(Sprite* sprite);

protected:
    vector<UIObject*>* children;
    float x;                // x coordinate in percentage of parent.
    float y;                // y coordinate in percentage of parent.
    float width;            // width in percentage of parent.
    float height;           // height in percentage of parent.
    vector<Sprite*>* sprites;
    bool changed = true;
};

class UIClickable : public UIObject {
public:
	using FuncType = std::function<void()>; // typedef for a function pointer
    UIClickable(float x, float y, float width, float height, vector<Sprite*>* sprites,
        FuncType click = DEFAULT_CLICK);
    virtual void activateClick();   // Calls the object's click function.
    virtual void releaseClick();    // Called when the mouse is released.
    virtual Rectangle update(float pX, float pY, float pWidth, float pHeight, State state = DEFAULT_STATE);
protected:
    FuncType click;                 // The function to call when clicked.
    bool isClicked = false;
};

class UIDraggable : public UIClickable {
public:
    UIDraggable(float x, float y, float width, float height, vector<Sprite*>* sprites,
        FuncType click = DEFAULT_CLICK);
    virtual Rectangle update(float pX, float pY, float pWidth, float pHeight, State state = DEFAULT_STATE);
};

class UIVertex : public UIDraggable {
public:
    UIVertex(float x, float y, float radius, string text = "", Color textColor = WHITE,
    int fontSize = 20);
    // TODO: These constructors are bad:
    UIVertex(float x, float y, float width, float height);   // Default sprite: Circle
    UIVertex(float x, float y, float width, float height, vector<Sprite*>* sprites);
    virtual void draw(float x, float y, float width, float height);
    virtual Rectangle update(float pX, float pY, float pWidth, float pHeight, State state = DEFAULT_STATE);
protected:
    string text;
    SText* textSprite;
};

class UIEdge : public UIObject {
public:
    UIEdge(UIVertex* vertex1, UIVertex* vertex2);             // Default sprite: Line
    UIEdge(UIVertex* vertex1, UIVertex* vertex2, vector<Sprite*>* sprites);
    virtual Rectangle update(float pX, float pY, float pWidth, float pHeight, State state = DEFAULT_STATE);
    
protected:
    UIVertex* vertex1;
    UIVertex* vertex2;
};

class UIGraph : public UIObject {
public:
    // UIGraph(float x, float y, float width, float height);   // Default sprite: Rectangle
    UIGraph(float x, float y, float width, float height, Graph* backendGraph = new Graph(),
        vector<Sprite*>* sprites = new vector<Sprite*>{ new SRectangle(BLACK) });
    virtual Rectangle update(float pX, float pY, float pWidth, float pHeight, State state = DEFAULT_STATE);
    void addVertex(float x, float y);
    void addEdge(int v1, int v2);
    vector<UIVertex*>* getVertices();
    vector<UIEdge*>* getEdges();
    void setVertices(vector<UIVertex*>* vertices);
    void setEdges(vector<UIEdge*>* edges);
    Graph* getBackendGraph();
protected:
    vector<UIVertex*>* vertices;
    vector<UIEdge*>* edges;
    Graph* backendGraph;
};

class UIToolbar : public UIObject {
public:
    UIToolbar(float x, float y, float width, float height);
    UIToolbar(float x, float y, float width, float height, vector<int>* tools, int curTool = 0);
    void setCurTool(int tool);
    int getCurTool();
private:
    vector<int>* tools;
    int curTool;
};

class UIApp {
public:
    UIApp(UIObject* curScreen);
    ~UIApp();
    void update();
protected:
    UIObject* curScreen;
};