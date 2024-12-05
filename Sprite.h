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

// Uses width/2 as radius, height does not affect anything.
class SCircle : public Sprite {
public:
    SCircle(Color backgroundColor);
    virtual void draw(float x, float y, float width, float height);
protected:
};

// Uses width/2 as radius, height does not affect anything.
class SSquare : public Sprite {
public:
    SSquare(Color backgroundColor);
    virtual void draw(float x, float y, float width, float height);
protected:
};

class SLine : public Sprite {
public:
    SLine(Color backgroundColor, float thickness = 1);
    virtual void draw(float x1, float y1, float x2, float y2);
protected:
    float thickness;
};