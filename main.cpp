#include <iostream>
#include <cstring>
#include "raylib.h"
#include "UI.h"
#include "Sprite.h"
#include "Graph.h"

#define WIDTH 720
#define HEIGHT 720

#define TB_WIDTH 0.07

using namespace std;

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WIDTH, HEIGHT, "Isperia: A Graph Theorist's Sketchpad");  // IMPORTANT NOTE: Must init window before doing any font stuff!

    UIGraph* gUI = new UIGraph(0, 0, 1 - TB_WIDTH, 1);
    Graph* g = new Graph(gUI);

    UIToolbar* tb = new UIToolbar(1 - TB_WIDTH + 0.001, 0, TB_WIDTH, 1, new vector<int>{0, 1, 2, 3});

    UIObject* mainScreen = new UIObject(0, 0, 1, 1);
    mainScreen->addChild(gUI);
    mainScreen->addChild(tb);

    g->print();
    cout << endl;

    g->addVertex(0.1, 0.1);
    g->addVertex(0.5, 0.5);
    g->print();
    cout << endl;

    g->addEdge(0, 1);
    g->print();
    cout << endl;

    g->addVertex(0.75, 0.1);
    g->addVertex(0.1, 0.75);
    g->print();
    cout << endl;

    g->addEdge(0, 3);
    g->addEdge(1, 3);
    g->addEdge(2, 3);
    g->print();
    cout << endl;

    // bool* connections = (bool*) calloc(5, sizeof(bool));

    // connections[1] = 1;
    // connections[3] = 1;

    // g->addVertex(connections);
    // g->print();
    // cout << endl;

    // g->addEdge(0, 2);
    // g->print();
    // cout << endl;

    // g->removeEdge(1, 4);
    // g->print();
    // cout << endl;

    UIApp* myApp = new UIApp(mainScreen);

    while (!WindowShouldClose()) {
        myApp->update();
    }
    CloseWindow();

    return 0;
}