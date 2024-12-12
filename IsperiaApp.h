#pragma once
#include "UI.h"
#include "Graph.h"

#define TB_WIDTH                0.07
#define MATRIX_WIDTH            0.2
#define COLOR_1                 GRAY
#define MATRIX_COLOR            COLOR_1
#define TEXT_HEIGHT             0.03
#define MATRIX_LABEL_HEIGHT     TEXT_HEIGHT
#define MATRIX_LABEL_COLOR      WHITE
#define MATRIX_LABEL_FONTSIZE   20
#define TOOLBAR_COLOR           COLOR_1 
#define GLOBAL_PADDING          0.01
#define INFOTEXT_HEIGHT         TEXT_HEIGHT
#define EIGENPROJ_COLOR         BLACK

class IsperiaApp : public UIApp {
public:
    IsperiaApp(Color backgroundColor = BLACK);
    void update();
protected:
    Color backgroundColor;
    UIGraph* gUI;
    UIEigenProjGraph* eigenProjGraph;
    UIObject* eigenProjGraphText;
    UIObject* mainScreen;
    UIToolbar* toolbar;
    UIMatrix* UIAdjMat;
    UIObject* adjMatText;
    UIMatrix* UILapMat;
    UIObject* lapMatText;
    UIMatrix* UIEigValMat;
    UIObject* eigValText;
    UIMatrix* UIEigVecMat;
    UIObject* eigVecText;
    UIInfoText* vertexCountText;
    UIInfoText* edgeCountText;
    UIInfoText* maxDegreeText;
    UIInfoText* minDegreeText;
};