#include <iostream>
#include "raylib.h"
#include "UI.h"
#include "Sprite.h"

#define WIDTH 720
#define HEIGHT 720

using namespace std;

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WIDTH, HEIGHT, "Isperia: A Graph Theorist's Sketchpad");  // IMPORTANT NOTE: Must init window before doing any font stuff!
    
    UIGraph* myGraph = new UIGraph(0, 0, 1, 1);
    UIVertex* v1 = new UIVertex(0.85, 0.85, 0.1, "1");
    UIVertex* v2 = new UIVertex(0.15, 0.15, 0.1, "2");
    UIEdge* e1 = new UIEdge(v1, v2, new vector<Sprite*>{ new SLine(WHITE, 5) });

    // SText* myText = new SText(YELLOW, "HELLO WORLD", 22);
    // v2->addSprite(myText);

    myGraph->setChildren(new vector<UIObject*>{ e1, v1, v2 });
    UIApp* myApp = new UIApp(myGraph);

    while (!WindowShouldClose()) {
        myApp->update();
    }
    CloseWindow();

    return 0;
}