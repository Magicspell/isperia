#pragma once
#include "UI.h"
#include "Graph.h"

#define TB_WIDTH                0.07
#define MATRIX_WIDTH            0.25
#define COLOR_1                 GRAY
#define MATRIX_COLOR            COLOR_1
#define MATRIX_LABEL_HEIGHT     0.03
#define MATRIX_LABEL_COLOR      WHITE
#define MATRIX_LABEL_FONTSIZE   20
#define TOOLBAR_COLOR           COLOR_1 
#define GLOBAL_PADDING          0.01

class IsperiaApp : public UIApp {
public:
    IsperiaApp(Color backgroundColor = BLACK);
    void update();
protected:
    Color backgroundColor;
    UIObject* mainScreen;
    UIToolbar* toolbar;
    UIMatrix* UIAdjMat;
    UIObject* adjMatText;
    UIMatrix* UILapMat;
    UIObject* lapMatText;
};