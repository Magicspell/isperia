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
    
    SRectangle* myRectangle = new SRectangle({ 200, 125, 230, 255 });
    SRectangle* myRectangle2 = new SRectangle({255, 0, 0, 255});
    SEllipse* myRectangle3 = new SEllipse(DARKGRAY);
    SLine* myLine = new SLine(BLACK, 5);

    UIObject* myobj = new UIObject(0, 0, 1, 1, myRectangle);
    UIDraggable* myButton = new UIDraggable(0.85, 0.85, 0.1, 0.05, myRectangle3);
    UIObject* myobj2 = new UIObject(0.25, 0.25, 0.5, 0.5, myRectangle2,
        new vector<UIObject*>{ myButton });

    UIObject* myObj3 = new UIObject(0, 0, 1, 1, myLine);

    myobj->setChildren(new vector<UIObject*>{  myObj3, myobj2 });
    UIApp* myApp = new UIApp(myobj);

    while (!WindowShouldClose()) {
        myApp->update();
    }
    CloseWindow();

    return 0;
}