#include <iostream>
#include "raylib.h"
#include "UI.h"

#define WIDTH 720
#define HEIGHT 720

using namespace std;


// class Button : public UIClickable {
// public:
//     Button(float x, float y, float width, float height, FuncType click,
//             Color backgroundColor = { 200, 125, 230, 255 }):
//             UIClickable(x, y, width, height, click, backgroundColor) {
//         this->click = []() {
//             cout << "BUTTON CLICKED!!!!" << endl;
//         };
//     }
// };

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WIDTH, HEIGHT, "Isperia: A Graph Theorist's Sketchpad");  // IMPORTANT NOTE: Must init window before doing any font stuff!
    
    UIObject* myobj = new UIObject(0, 0, 1, 1);
    UIObject* myobj2 = new UIObject(0.25, 0.25, 0.5, 0.5, {255, 0, 0, 255});
    UIClickable* myButton = new UIClickable(0.85, 0.85, 0.1, 0.05, []() {
        cout << "BUTTON CLICKED!!!" << endl;
    }, DARKGRAY);

    myobj->children = new vector<UIObject*>{ myobj2, myButton };
    UIApp* myApp = new UIApp(myobj);

    while (!WindowShouldClose()) {
        myApp->update();
    }
    CloseWindow();

    return 0;
}