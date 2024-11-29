#include <iostream>
#include "raylib.h"

#define WIDTH 720
#define HEIGHT 720

using namespace std;

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WIDTH, HEIGHT, "raylib [core] example - basic window");  // IMPORTANT NOTE: Must init window before doing any font stuff!

    while (!WindowShouldClose())
    {
        BeginDrawing();
        EndDrawing();
    }
    
    return 0;
}