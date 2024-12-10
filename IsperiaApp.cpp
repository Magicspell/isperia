#include "IsperiaApp.h"

IsperiaApp::IsperiaApp(Color backgroundColor): UIApp(new UIObject(0, 0, 1, 1)),
        backgroundColor(backgroundColor) {
    UIGraph* gUI = new UIGraph(ADJMAT_WIDTH, 0, 1 - TB_WIDTH - ADJMAT_WIDTH, 1);

    this->toolbar = new UIToolbar(1 - TB_WIDTH + 0.001, 0, TB_WIDTH, 1, new vector<int>{
        SELECT,
        ADD_VERTEX,
        ADD_EDGE,
        REMOVE_VERTEX
    }, ADD_VERTEX, new vector<Sprite*>{new SRectangle(TOOLBAR_COLOR)});

    this->UIAdjMat = new UIMatrix(0, 0, ADJMAT_WIDTH, ADJMAT_WIDTH, gUI->getBackendGraph()->getAdjMat(),
        new vector<Sprite*>{ new SRectangle(MATRIX_COLOR) });

    this->mainScreen = this->curScreen;     // SCUFFED
    this->mainScreen->addChild(gUI);
    this->mainScreen->addChild(this->toolbar);
    this->mainScreen->addChild(this->UIAdjMat);
}

void IsperiaApp::update() {
    BeginDrawing();
    ClearBackground(this->backgroundColor);
    this->curScreen->update(0, 0, GetScreenWidth(), GetScreenHeight(), {
        this->toolbar->getCurTool()
    });
    EndDrawing();
}