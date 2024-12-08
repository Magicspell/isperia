#include "IsperiaApp.h"

IsperiaApp::IsperiaApp(): UIApp(new UIObject(0, 0, 1, 1)) {
    UIGraph* gUI = new UIGraph(0, 0, 1 - TB_WIDTH, 1);

    this->toolbar = new UIToolbar(1 - TB_WIDTH + 0.001, 0, TB_WIDTH, 1, new vector<int>{
        SELECT,
        ADD_VERTEX,
        ADD_EDGE
    });

    this->mainScreen = this->curScreen;     // SCUFFED
    this->mainScreen->addChild(gUI);
    this->mainScreen->addChild(this->toolbar);

    // gUI->getBackendGraph()->print();
    // cout << endl;

    // gUI->addVertex(0.1, 0.1);
    // gUI->addVertex(0.5, 0.5);
    // gUI->getBackendGraph()->print();
    // cout << endl;

    // gUI->addEdge(0, 1);
    // gUI->getBackendGraph()->print();
    // cout << endl;

    // gUI->addVertex(0.75, 0.1);
    // gUI->addVertex(0.1, 0.75);
    // gUI->getBackendGraph()->print();
    // cout << endl;

    // gUI->addEdge(0, 3);
    // gUI->addEdge(1, 3);
    // gUI->addEdge(2, 3);
    // gUI->getBackendGraph()->print();
    // cout << endl;
}

void IsperiaApp::update() {
    BeginDrawing();
    this->curScreen->update(0, 0, GetScreenWidth(), GetScreenHeight(), {
        this->toolbar->getCurTool()
    });
    EndDrawing();
}