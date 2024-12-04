#include <iostream>
#include "raylib.h"
#include "UI.h"

#define WIDTH 720
#define HEIGHT 720

using namespace std;

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WIDTH, HEIGHT, "Isperia: A Graph Theorist's Sketchpad");  // IMPORTANT NOTE: Must init window before doing any font stuff!
    
    UIObject* myobj = new UIObject(0, 0, 1, 1);
    UIObject* myobj2 = new UIObject(0.25, 0.25, 0.5, 0.5, {255, 0, 0, 255});
    UIDraggable* myButton = new UIDraggable(0.85, 0.85, 0.1, 0.05, [myobj2]() {
        myobj2->children->push_back(new UIObject(0.33, 0.33, 0.33, 0.33, PINK));
    }, DARKGRAY);

    myobj->children = new vector<UIObject*>{ myobj2, myButton };
    UIApp* myApp = new UIApp(myobj);

    while (!WindowShouldClose()) {
        myApp->update();
    }
    CloseWindow();

    return 0;
}