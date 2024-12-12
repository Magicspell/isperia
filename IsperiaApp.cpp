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
            REMOVE_VERTEX,
            REMOVE_EDGE
        },
        ADD_VERTEX,
        new vector<Sprite*>{new SRectangle(TOOLBAR_COLOR)},
        new vector<Sprite*>{
            new SCursor(WHITE),
            new SVertex(RED)
        }
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
        GLOBAL_PADDING * 2 + TEXT_HEIGHT,
        MATRIX_WIDTH,
        MATRIX_WIDTH,
        this->gUI->getBackendGraph(),
        new vector<Sprite*>{ new SRectangle(EIGENPROJ_COLOR) }
    );

    this->eigValText = new UIObject(
        1 - MATRIX_WIDTH - GLOBAL_PADDING,
        GLOBAL_PADDING * 2 + TEXT_HEIGHT + MATRIX_WIDTH,
        MATRIX_WIDTH,
        MATRIX_LABEL_HEIGHT,
        new vector<Sprite*>{ new SText(MATRIX_LABEL_COLOR, "Eigenvalues",
            MATRIX_LABEL_FONTSIZE, LEFT) }
    );

    this->UIEigValMat = new UIMatrix(
        1 - MATRIX_WIDTH - GLOBAL_PADDING,
        GLOBAL_PADDING * 3 + TEXT_HEIGHT * 2 + MATRIX_WIDTH,
        MATRIX_WIDTH,
        MATRIX_WIDTH,
        this->gUI->getBackendGraph(),
        EIG_VAL,
        new vector<Sprite*>{ new SRectangle(MATRIX_COLOR) }
    );

    this->eigVecText = new UIObject(
        1 - MATRIX_WIDTH - GLOBAL_PADDING,
        GLOBAL_PADDING * 4 + TEXT_HEIGHT * 2 + MATRIX_WIDTH * 2,
        MATRIX_WIDTH,
        MATRIX_LABEL_HEIGHT,
        new vector<Sprite*>{ new SText(MATRIX_LABEL_COLOR, "Eigenvectors", MATRIX_LABEL_FONTSIZE, LEFT) }
    );

    this->UIEigVecMat = new UIMatrix(
        1 - MATRIX_WIDTH - GLOBAL_PADDING,
        GLOBAL_PADDING * 4 + TEXT_HEIGHT * 3 + MATRIX_WIDTH * 2,
        MATRIX_WIDTH,
        MATRIX_WIDTH,
        this->gUI->getBackendGraph(),
        EIG_VEC,
        new vector<Sprite*>{ new SRectangle(MATRIX_COLOR) }
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

    this->maxDegreeText = new UIInfoText(
        GLOBAL_PADDING,
        GLOBAL_PADDING * 3 + INFOTEXT_HEIGHT * 2,
        MATRIX_WIDTH,
        INFOTEXT_HEIGHT,
        [this](){ return to_string(this->gUI->getBackendGraph()->getMaxDegree()); },
        "Max Degree: "
    );

    this->minDegreeText = new UIInfoText(
        GLOBAL_PADDING,
        GLOBAL_PADDING * 4 + INFOTEXT_HEIGHT * 3,
        MATRIX_WIDTH,
        INFOTEXT_HEIGHT,
        [this](){ return to_string(this->gUI->getBackendGraph()->getMinDegree()); },
        "Min Degree: "
    );

    this->UIAdjMat = new UIMatrix(
        GLOBAL_PADDING,
        GLOBAL_PADDING * 5 + MATRIX_LABEL_HEIGHT + INFOTEXT_HEIGHT * 4,
        MATRIX_WIDTH,
        MATRIX_WIDTH,
        this->gUI->getBackendGraph(),
        ADJ,
        new vector<Sprite*>{ new SRectangle(MATRIX_COLOR) }
    );
    
    this->adjMatText = new UIObject(
        GLOBAL_PADDING,
        GLOBAL_PADDING * 5 + INFOTEXT_HEIGHT * 4,
        MATRIX_WIDTH,
        MATRIX_LABEL_HEIGHT,
        new vector<Sprite*>{ new SText(MATRIX_LABEL_COLOR, "Adjacency Matrix", MATRIX_LABEL_FONTSIZE, LEFT) }
    );

    this->UILapMat = new UIMatrix(
        GLOBAL_PADDING,
        GLOBAL_PADDING * 6 + MATRIX_LABEL_HEIGHT * 2 + MATRIX_WIDTH + INFOTEXT_HEIGHT * 4,
        MATRIX_WIDTH,
        MATRIX_WIDTH,
        this->gUI->getBackendGraph(),
        LAP,
        new vector<Sprite*>{ new SRectangle(MATRIX_COLOR) }
    );
    
    this->lapMatText = new UIObject(
        GLOBAL_PADDING,
        GLOBAL_PADDING * 6 + MATRIX_LABEL_HEIGHT + MATRIX_WIDTH + INFOTEXT_HEIGHT * 4,
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
    this->mainScreen->addChild(this->UIEigVecMat);
    this->mainScreen->addChild(this->eigVecText);
    this->mainScreen->addChild(this->UIEigValMat);
    this->mainScreen->addChild(this->eigValText);
    this->mainScreen->addChild(this->vertexCountText);
    this->mainScreen->addChild(this->edgeCountText);
    this->mainScreen->addChild(this->maxDegreeText);
    this->mainScreen->addChild(this->minDegreeText);
}

void IsperiaApp::update() {
    BeginDrawing();
    ClearBackground(this->backgroundColor);
    this->curScreen->update(0, 0, GetScreenWidth(), GetScreenHeight(), {
        this->toolbar->getCurTool()
    });
    EndDrawing();
}