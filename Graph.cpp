#include <cstdlib>
#include <cstring>
#include <iostream>
#include "Graph.h"

using namespace std;

Graph::Graph(int size): size(size) {
    this->adjMat = (int**) calloc(size, sizeof(int*));
    this->lapMat = (int**) calloc(size, sizeof(int*));

    for (int i = 0; i < size; i++) {
        this->adjMat[i] = (int*) calloc(size, sizeof(int));
        this->lapMat[i] = (int*) calloc(size, sizeof(int));
    }
}

Graph::Graph(): Graph(0) {}

Graph::~Graph() {
    for (int i = 0; i < this->size; i++) {
        delete this->adjMat[i];
        delete this->lapMat[i];
    }

    delete this->adjMat;
    delete this->lapMat;
}

void Graph::addVertex(float x, float y) {
    this->addVertex(x, y, (int*) calloc(this->size + 1, sizeof(int)));
}

void Graph::addVertex(float x, float y, int* connections) {
    this->size += 1;

    int** newAdj = (int**) calloc(this->size, sizeof(int*));    // Allocate new space.

    for (int i = 0; i < this->size - 1; i++) {
        newAdj[i] = (int*) calloc(this->size, sizeof(int));     // Allocate row.
        memcpy(newAdj[i], this->adjMat[i], this->size - 1);     // Copy from old matrix.
        newAdj[i][this->size - 1] = connections[i];             // Set connections for last
                                                                // element.
    }
    newAdj[this->size - 1] = connections;                       // Add last row (connections).

    delete this->adjMat;                                        // TODO: Wrong, need to delete all rows
    this->adjMat = newAdj;

    this->updateLapMat();
}

void Graph::removeVertex(int index) {
    this->size -= 1;

    int** newAdj = (int**) calloc(this->size, sizeof(int*));            // Allocate new space.

    for (int i = 0; i < this->size + 1; i++) {
        if (i != index) {
            int curI = i;
            if (i > index) curI -= 1;

            newAdj[curI] = (int*) calloc(this->size, sizeof(int));      // Allocate row.
            for (int j = 0; j < this->size + 1; j++) {                  // Copy from old matrix.
                if (j != index) {                                       // Make sure we dont copy element from
                    int curJ = j;                                       // column we are removing.
                    if (j > index) curJ -= 1;

                    newAdj[curI][curJ] = this->adjMat[i][j];
                } 
            }
        }
    }

    delete this->adjMat;                                        // TODO: Wrong, need to delete all rows
    this->adjMat = newAdj;

    this->updateLapMat();
}

// Adds an edge between two vertices, provided by indices of the adjacency matrix.
void Graph::addEdge(int v1, int v2) {
    this->adjMat[v1][v2] = 1;
    this->adjMat[v2][v1] = 1;

    this->updateLapMat();
}

// Removes an edge between two vertices, provided by indexes of the adjacency matrix.
void Graph::removeEdge(int v1, int v2) {
    this->adjMat[v1][v2] = 0;
    this->adjMat[v2][v1] = 0;

    this->updateLapMat();
}

void Graph::print() {
    for (int x = 0; x < this->size; x++) {
        for (int y = 0; y < this->size; y++) {
            cout << this->adjMat[x][y] << " ";
        }
        cout << endl;
    }
}

int Graph::getSize() {
    return this->size;
}

int** Graph::getAdjMat() {
    return this->adjMat;
}

int** Graph::getLapMat() {
    return this->lapMat;
}

int** Graph::getMatByType(MatType matType) {
    switch (matType) {
    case ADJ:
        return this->getAdjMat();
    case LAP:
        return this->getLapMat();
    }
}

void Graph::updateLapMat() {
    // Calculate the laplacian matrix: L = D - A

    int** newLap = (int**) calloc(this->size, sizeof(int*));            // Allocate new space.
    for (int i = 0; i < this->size; i++) {
        newLap[i] = (int*) calloc(this->size, sizeof(int));             // Allocate row.
        
        // Calculate degree (just sum of adj row)
        int degree = 0;
        for (int di = 0; di < this->size; di++) { degree += this->adjMat[i][di]; }

        // Set diagonal to degree
        newLap[i][i] = degree;

        // Set other values to D - A (which just becomes 0 - A, so just (-1) * A)
        for (int j = 0; j < this->size; j++) {
            if (j != i) {
                newLap[i][j] = -1 * this->adjMat[i][j];
            }
        }
    }

    // Delete old TODO: might not be deleting elements.
    delete this->lapMat;
    this->lapMat = newLap;
}