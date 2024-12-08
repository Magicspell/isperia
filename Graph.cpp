#include <cstdlib>
#include <cstring>
#include <iostream>
#include "Graph.h"

using namespace std;

Graph::Graph(UIGraph* graphUI, int size): size(size), graphUI(graphUI) {
    this->adjMat = (bool**) calloc(size, sizeof(bool*));

    for (int i = 0; i < size; i++) {
        this->adjMat[i] = (bool*) calloc(size, sizeof(bool));
    }
}

Graph::Graph(int size): Graph(new UIGraph(0, 0, 1, 1), size) {}

Graph::Graph(UIGraph* graphUI): Graph(graphUI, 0) {}

Graph::Graph(): Graph(new UIGraph(0, 0, 1, 1), 0) {}

Graph::~Graph() {
    for (int i = 0; i < this->size; i++) {
        delete this->adjMat[i];
    }

    delete this->adjMat;
}

void Graph::addVertex(float x, float y) {
    this->addVertex(x, y, (bool*) calloc(this->size + 1, sizeof(bool)));
}

void Graph::addVertex(float x, float y, bool* connections) {
    this->graphUI->addVertex(new UIVertex(x, y, 0.1, to_string(this->size)));
    this->size += 1;

    bool** newAdj = (bool**) calloc(this->size, sizeof(bool*)); // Allocate new space.

    for (int i = 0; i < this->size - 1; i++) {
        newAdj[i] = (bool*) calloc(this->size, sizeof(bool));   // Allocate row.
        memcpy(newAdj[i], this->adjMat[i], this->size - 1);     // Copy from old matrix.
        newAdj[i][this->size - 1] = connections[i];             // Set connections for last
                                                                // element.
    }
    newAdj[this->size - 1] = connections;                       // Add last row (connections).

    delete this->adjMat;                                        // TODO: Wrong, need to delete all rows
    this->adjMat = newAdj;
}

// void Graph::removeVertex(int index) {
//     if (this->size < 1) { return; }

//     this->size -= 1;

//     bool** newAdj = (bool**) calloc(this->size, sizeof(bool*)); // Allocate new space.

//     for (int i = 0; i < this->size - 1; i++) {
//         if (i != index) {
//             newAdj[i] = (bool*) calloc(this->size, sizeof(bool));   // Allocate row.
//             memcpy(newAdj[i], this->adjMat[i], this->size - 1);     // Copy from old matrix.
//         }
//     }

//     delete this->adjMat;                                        // TODO: Wrong, need to delete all rows
//     this->adjMat = newAdj;
// }

// Adds an edge between two vertices, provided by indexes of the adjacency matrix.
void Graph::addEdge(int v1, int v2) {
    this->graphUI->addEdge(new UIEdge(this->getUIVertex(v1), this->getUIVertex(v2)));

    this->adjMat[v1][v2] = 1;
    this->adjMat[v2][v1] = 1;
}

// Removes an edge between two vertices, provided by indexes of the adjacency matrix.
void Graph::removeEdge(int v1, int v2) {
    this->adjMat[v1][v2] = 0;
    this->adjMat[v2][v1] = 0;
}

UIGraph* Graph::getUIGraph() {
    return this->graphUI;
}

UIVertex* Graph::getUIVertex(int index) {
    return this->graphUI->getVertices()->at(index);
}

// void Graph::updateUIGraph() {
//     // Delete all UIVertex and UIEdges
//     for (UIVertex* v : *(this->getUIGraph()->getVertices())) {
//         delete v;
//     }
//     for (UIEdge* e : *(this->getUIGraph()->getEdges())) {
//         delete e;
//     }
//     // Dont know if this is nessiscary.
//     delete this->getUIGraph()->getVertices();
//     delete this->getUIGraph()->getEdges();

//     // Add 
//     vector<UIVertex*>* newVertices = new vector<UIVertex*>();
//     vector<UIEdge*>* newEdges = new vector<UIEdge*>();

// }

void Graph::print() {
    for (int x = 0; x < this->size; x++) {
        for (int y = 0; y < this->size; y++) {
            cout << this->adjMat[x][y] << " ";
        }
        cout << endl;
    }
}