#pragma once
#include "raylib.h"

// "Abstract" class for sprites
class Sprite {
public:
    Sprite(Color backgroundColor);
    virtual void draw(float x, float y, float width, float height);
protected:
    Color backgroundColor;
};

class SRectangle : public Sprite {
public:
    SRectangle(Color backgroundColor);
    virtual void draw(float x, float y, float width, float height);
protected:
};

class SEllipse : public Sprite {
public:
    SEllipse(Color backgroundColor);
    virtual void draw(float x, float y, float width, float height);
protected:
};