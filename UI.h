#pragma once
#include <vector>
#include <functional>
#include "raylib.h"

using namespace std;

class UIObject {
public:
    UIObject(float x, float y, float width, float height, Color backgroundColor = { 200, 125, 230, 255 });
    ~UIObject();
    virtual void draw(float x, float y, float width, float height);
    // virtual void _draw();
    virtual Rectangle update(float pX, float pY, float pWidth, float pHeight);
    bool getChanged();
    vector<UIObject*>* children;
protected:
    float x;                // x coordinate in percentage of parent.
    float y;                // y coordinate in percentage of parent.
    float width;            // Width in percentage of parent.
    float height;           // Height in percentage of parent.
    Color backgroundColor;
    bool changed = true;
};

class UIClickable : public UIObject {
public:
	using FuncType = std::function<void()>; // typedef for a function pointer
    UIClickable(float x, float y, float width, float height, FuncType click, Color backgroundColor = { 200, 125, 230, 255 });
    void activateClick();   // Calls the object's click function.
    virtual Rectangle update(float pX, float pY, float pWidth, float pHeight);
protected:
    FuncType click;         // The function to call when clicked.
};

class UIApp {
public:
    UIApp(UIObject* curScreen);
    ~UIApp();
    void update();
protected:
    UIObject* curScreen;
};