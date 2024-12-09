#include "IsperiaApp.h"

IsperiaApp::IsperiaApp(): UIApp(new UIObject(0, 0, 1, 1)) {
    UIGraph* gUI = new UIGraph(ADJMAT_WIDTH, 0, 1 - TB_WIDTH - ADJMAT_WIDTH, 1);

    this->toolbar = new UIToolbar(1 - TB_WIDTH + 0.001, 0, TB_WIDTH, 1, new vector<int>{
        SELECT,
        ADD_VERTEX,
        ADD_EDGE,
        REMOVE_VERTEX
    });

    this->UIAdjMat = new UIMatrix(0, 0, ADJMAT_WIDTH, ADJMAT_WIDTH, gUI->getBackendGraph()->getAdjMat(),
        new vector<Sprite*>{ new SRectangle(WHITE) });

    this->mainScreen = this->curScreen;     // SCUFFED
    this->mainScreen->addChild(gUI);
    this->mainScreen->addChild(this->toolbar);
    this->mainScreen->addChild(this->UIAdjMat);
}

void IsperiaApp::update() {
    BeginDrawing();
    this->curScreen->update(0, 0, GetScreenWidth(), GetScreenHeight(), {
        this->toolbar->getCurTool()
    });
    EndDrawing();
}