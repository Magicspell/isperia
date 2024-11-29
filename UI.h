#pragma once
#include <vector>
#include "raylib.h"

using namespace std;

class UIObject {
public:
    UIObject(float x, float y, float width, float height, Color backgroundColor = { 200, 125, 230, 255 });
    ~UIObject();
    virtual void draw(float pX, float pY, float pWidth, float pHeight);
    virtual void _draw(float pX, float pY, float pWidth, float pHeight);
    bool getChanged();
    vector<UIObject*>* children;
private:
    float x;
    float y;
    float width;
    float height;
    // Color backgroundColor = {200, 125, 230, 255};
    Color backgroundColor;
    bool changed = true;
};

class UIApp {
public:
    UIApp(UIObject* curScreen);
    ~UIApp();
    void update();
private:
    UIObject* curScreen;
};