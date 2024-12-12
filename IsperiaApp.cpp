#include "IsperiaApp.h"

IsperiaApp::IsperiaApp(Color backgroundColor): UIApp(new UIObject(0, 0, 1, 1)),
        backgroundColor(backgroundColor) {
    
    this->gUI = new UIGraph(
        MATRIX_WIDTH + GLOBAL_PADDING * 2,
        GLOBAL_PADDING,
        1 - TB_WIDTH - MATRIX_WIDTH * 2 - GLOBAL_PADDING * 4,
        1
    );

    this->toolbar = new UIToolbar(
        1 - TB_WIDTH - GLOBAL_PADDING * 2 - MATRIX_WIDTH,
        GLOBAL_PADDING,
        TB_WIDTH,
        1 - GLOBAL_PADDING * 2,
        new vector<int>{
            SELECT,
            ADD_VERTEX,
            ADD_EDGE,
            REMOVE_VERTEX
        },
        ADD_VERTEX,
        new vector<Sprite*>{new SRectangle(TOOLBAR_COLOR)}
    );

    this->eigenProjGraphText = new UIObject(
        1 - MATRIX_WIDTH - GLOBAL_PADDING,
        GLOBAL_PADDING,
        MATRIX_WIDTH,
        TEXT_HEIGHT,
        new vector<Sprite*>{ new SText(WHITE, "Eigenvector Projection") }
    );

    this->eigenProjGraph = new UIEigenProjGraph(
        1 - MATRIX_WIDTH - GLOBAL_PADDING,
        GLOBAL_PADDING* 2 + TEXT_HEIGHT,
        MATRIX_WIDTH,
        MATRIX_WIDTH,
        this->gUI->getBackendGraph(),
        new vector<Sprite*>{ new SRectangle(EIGENPROJ_COLOR) }
    );

    this->vertexCountText = new UIInfoText(
        GLOBAL_PADDING,
        GLOBAL_PADDING,
        MATRIX_WIDTH,
        INFOTEXT_HEIGHT,
        [this](){ return to_string(this->gUI->getBackendGraph()->getSize()); },
        "Vertex Count: "
    );

    this->edgeCountText = new UIInfoText(
        GLOBAL_PADDING,
        GLOBAL_PADDING * 2 + INFOTEXT_HEIGHT,
        MATRIX_WIDTH,
        INFOTEXT_HEIGHT,
        [this](){ return to_string(this->gUI->getBackendGraph()->getEdgeCount()); },
        "Edge Count: "
    );

    this->UIAdjMat = new UIMatrix(
        GLOBAL_PADDING,
        GLOBAL_PADDING * 3 + MATRIX_LABEL_HEIGHT + INFOTEXT_HEIGHT * 2,
        MATRIX_WIDTH,
        MATRIX_WIDTH,
        this->gUI->getBackendGraph(),
        ADJ,
        new vector<Sprite*>{ new SRectangle(MATRIX_COLOR) }
    );
    
    this->adjMatText = new UIObject(
        GLOBAL_PADDING,
        GLOBAL_PADDING * 3 + INFOTEXT_HEIGHT * 2,
        MATRIX_WIDTH,
        MATRIX_LABEL_HEIGHT,
        new vector<Sprite*>{ new SText(MATRIX_LABEL_COLOR, "Adjacency Matrix", MATRIX_LABEL_FONTSIZE, LEFT) }
    );

    this->UILapMat = new UIMatrix(
        GLOBAL_PADDING,
        GLOBAL_PADDING * 4 + MATRIX_LABEL_HEIGHT * 2 + MATRIX_WIDTH + INFOTEXT_HEIGHT * 2,
        MATRIX_WIDTH,
        MATRIX_WIDTH,
        this->gUI->getBackendGraph(),
        LAP,
        new vector<Sprite*>{ new SRectangle(MATRIX_COLOR) }
    );
    
    this->lapMatText = new UIObject(
        GLOBAL_PADDING,
        GLOBAL_PADDING * 4 + MATRIX_LABEL_HEIGHT + MATRIX_WIDTH + INFOTEXT_HEIGHT * 2,
        MATRIX_WIDTH,
        MATRIX_LABEL_HEIGHT,
        new vector<Sprite*>{ new SText(MATRIX_LABEL_COLOR, "Laplacian Matrix", MATRIX_LABEL_FONTSIZE, LEFT) }
    );

    this->mainScreen = this->curScreen;     // SCUFFED
    this->mainScreen->addChild(this->gUI);
    this->mainScreen->addChild(this->eigenProjGraph);
    this->mainScreen->addChild(this->eigenProjGraphText);
    this->mainScreen->addChild(this->toolbar);
    this->mainScreen->addChild(this->UIAdjMat);
    this->mainScreen->addChild(this->adjMatText);
    this->mainScreen->addChild(this->UILapMat);
    this->mainScreen->addChild(this->lapMatText);
    this->mainScreen->addChild(this->vertexCountText);
    this->mainScreen->addChild(this->edgeCountText);
}

void IsperiaApp::update() {
    BeginDrawing();
    ClearBackground(this->backgroundColor);
    this->curScreen->update(0, 0, GetScreenWidth(), GetScreenHeight(), {
        this->toolbar->getCurTool()
    });
    EndDrawing();
}