#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cmath>
#include "Graph.h"

#define ROUND_DIGITS 2

using namespace std;

Graph::Graph(int size): size(size) {
    this->adjMat = (int**) calloc(size, sizeof(int*));
    this->lapMat = (int**) calloc(size, sizeof(int*));

    for (int i = 0; i < size; i++) {
        this->adjMat[i] = (int*) calloc(size, sizeof(int));
        this->lapMat[i] = (int*) calloc(size, sizeof(int));
    }
    this->updateEdgeCount();
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

// Called whenever the graph is changed.
void Graph::update() {
    this->updateLapMat();
    this->updateEdgeCount();
    if (this->size > 0) {
        this->updateEigen();
        this->updateEigenCoords();
    }
}

void Graph::addVertex(float x, float y) {
    this->addVertex(x, y, (int*) calloc(this->size + 1, sizeof(int)));
}

void Graph::addVertex(float x, float y, int* connections) {
    this->size += 1;

    int** newAdj = (int**) calloc(this->size, sizeof(int*));                    // Allocate new space.

    for (int i = 0; i < this->size - 1; i++) {
        newAdj[i] = (int*) calloc(this->size, sizeof(int));                     // Allocate row.
        memcpy(newAdj[i], this->adjMat[i], (this->size - 1) * sizeof(int));     // Copy from old matrix.
        newAdj[i][this->size - 1] = connections[i];                             // Set connections for last
                                                                                // element.
    }
    newAdj[this->size - 1] = connections;                                       // Add last row (connections).

    delete this->adjMat;    // TODO: Wrong, need to delete all rows
    this->adjMat = newAdj;

    this->update();
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

    this->update();
}

// Adds an edge between two vertices, provided by indices of the adjacency matrix.
void Graph::addEdge(int v1, int v2) {
    this->adjMat[v1][v2] = 1;
    this->adjMat[v2][v1] = 1;

    this->update();
}

// Removes an edge between two vertices, provided by indexes of the adjacency matrix.
void Graph::removeEdge(int v1, int v2) {
    this->adjMat[v1][v2] = 0;
    this->adjMat[v2][v1] = 0;

    this->update();
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

void Graph::updateEdgeCount() {
    // Divide total degree by 2
    int degree = 0;
    for (int i = 0; i < this->size; i++) {
        for (int j = 0; j < this->size; j++) {
            degree += this->adjMat[i][j];
        }
    }

    this->edgeCount = degree / 2;
}

int Graph::getEdgeCount() {
    return this->edgeCount;
}

void Graph::updateEigen() {
    Eigen::MatrixXf myMatrix(this->size, this->size);
    for (int i = 0; i < this->size; i++) {
        for (int j = 0; j < this->size; j++) {
            myMatrix(i, j) = (float) this->adjMat[i][j];
        }
    }

    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXf> solver(myMatrix);
    // TODO: FIX LEAKS
    this->eigenValues = new Eigen::VectorXf(solver.eigenvalues());
    this->eigenVectors = new Eigen::MatrixXf(solver.eigenvectors());
}

// Gets the indices of the two smallest non-zero eigenvalues.
int* Graph::getSmallestEigIndices() {
    
    int* lowestIndices = new int[2]{0, 0};
    float* lowestValues = new float[2]{FLT_MAX, FLT_MAX};

    int i = -1;
    for (float e : *(this->eigenValues)) {
        i++;
        float rounded = round(e * pow(10, ROUND_DIGITS)) / pow(10, ROUND_DIGITS);
        
        if (rounded <= 0) continue;
        if (rounded < lowestValues[0]) {
            lowestIndices[1] = lowestIndices[0];
            lowestIndices[0] = i;

            lowestValues[1] = lowestValues[0];
            lowestValues[0] = e;
        }
        else if (rounded < lowestValues[1]) {
            lowestIndices[1] = i;
            lowestValues[1] = e;
        }
    }

    return lowestIndices;
}

float** Graph::getEigenCoords() {
    return this->eigenCoords;
}

// Uses the eigenvectors of the adjacency matrix to get 2d cartesian coordinates.
void Graph::updateEigenCoords() {
    int* indices = this->getSmallestEigIndices();

    Eigen::VectorXf xCoords = (*(this->eigenVectors))(Eigen::all, indices[0]);
    Eigen::VectorXf yCoords = (*(this->eigenVectors))(Eigen::all, indices[1]);

    float** newCoords = (float**) calloc(this->size, sizeof(float*));
    for (int i = 0; i < this->size; i++) {
        newCoords[i] = (float*) calloc(2, sizeof(float));

        newCoords[i][0] = xCoords[i];
        newCoords[i][1] = yCoords[i];
    }

    this->eigenCoords = newCoords;
}

int Graph::getMaxDegree() {
    int ret = 0;
    for (int i = 0; i < this->size; i++) {
        int d = 0;
        for (int j = 0; j < this->size; j++) {
            d += this->adjMat[i][j];
        }
        if (d > ret) ret = d;
    }
    return ret;
}

int Graph::getMinDegree() {
    int ret = INT_MAX;
    for (int i = 0; i < this->size; i++) {
        int d = 0;
        for (int j = 0; j < this->size; j++) {
            d += this->adjMat[i][j];
        }
        if (d < ret) ret = d;
    }
    if (ret == INT_MAX) ret = 0;
    return ret;
}