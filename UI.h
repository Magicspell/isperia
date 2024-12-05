#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include "raylib.h"
#include "Sprite.h"

#define DEFAULT_COLOR { 200, 125, 230, 255 }
#define DEFAULT_CLICK [](){}    // Lambda that does nothing

using namespace std;

class UIObject {
public:
    UIObject(float x, float y, float width, float height, Sprite* sprite);
    ~UIObject();
    virtual void draw(float x, float y, float width, float height);
    virtual Rectangle update(float pX, float pY, float pWidth, float pHeight);
    bool getChanged();
    vector<UIObject*>* children;
protected:
    float x;                // x coordinate in percentage of parent.
    float y;                // y coordinate in percentage of parent.
    float width;            // Width in percentage of parent.
    float height;           // Height in percentage of parent.
    // Color backgroundColor;
    Sprite* sprite;
    bool changed = true;
};

class UIClickable : public UIObject {
public:
	using FuncType = std::function<void()>; // typedef for a function pointer
    UIClickable(float x, float y, float width, float height, Sprite* sprite,
        FuncType click = DEFAULT_CLICK);
    virtual void activateClick();   // Calls the object's click function.
    virtual void releaseClick();    // Called when the mouse is released.
    virtual Rectangle update(float pX, float pY, float pWidth, float pHeight);
protected:
    FuncType click;                 // The function to call when clicked.
    bool isClicked = false;
};

class UIDraggable : public UIClickable {
public:
    UIDraggable(float x, float y, float width, float height, Sprite* sprite,
        FuncType click = DEFAULT_CLICK);
    virtual Rectangle update(float pX, float pY, float pWidth, float pHeight);
};

class UIApp {
public:
    UIApp(UIObject* curScreen);
    ~UIApp();
    void update();
protected:
    UIObject* curScreen;
};