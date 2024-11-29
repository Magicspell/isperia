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
    myobj->children = new vector<UIObject*>{ myobj2 };
    UIApp* myApp = new UIApp(myobj);

    while (!WindowShouldClose())
    {   
        myApp->update();
    }
    CloseWindow();
    
    return 0;
}