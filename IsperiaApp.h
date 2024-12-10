#pragma once
#include "UI.h"
#include "Graph.h"

#define TB_WIDTH 0.07
#define ADJMAT_WIDTH 0.25
#define COLOR_1         GRAY
#define MATRIX_COLOR    COLOR_1
#define TOOLBAR_COLOR   COLOR_1

class IsperiaApp : public UIApp {
public:
    IsperiaApp(Color backgroundColor = BLACK);
    void update();
protected:
    Color backgroundColor;
    UIObject* mainScreen;
    UIToolbar* toolbar;
    UIMatrix* UIAdjMat;
};