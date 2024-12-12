#include <iostream>
#include <cstring>
#include "raylib.h"
#include "UI.h"
#include "Sprite.h"
#include "Graph.h"
#include "IsperiaApp.h"
#include "Eigen/Dense"


#define WIDTH 1080
#define HEIGHT 720

using namespace std;

int main() {
    // // Define a symmetric matrix
    // Eigen::Matrix3d A;
    // A << 2, -1, 0,
    //     -1, 2, -1,
    //      0, -1, 2;

    // // Compute eigenvalues and eigenvectors
    // Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solver(A);

    // // Check for successful computation
    // if (solver.info() != Eigen::Success) {
    //     std::cerr << "Eigenvalue decomposition failed!" << std::endl;
    //     return -1;
    // }

    // // Display eigenvalues
    // std::cout << "Eigenvalues:\n" << solver.eigenvalues() << std::endl;

    // // Display eigenvectors
    // std::cout << "Eigenvectors:\n" << solver.eigenvectors() << std::endl;

    // return 0;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WIDTH, HEIGHT, "Isperia: A Graph Theorist's Sketchpad");  // IMPORTANT NOTE: Must init window before doing any font stuff!


    
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

    IsperiaApp* myApp = new IsperiaApp();

    while (!WindowShouldClose()) {
        myApp->update();
    }
    CloseWindow();

    return 0;
}