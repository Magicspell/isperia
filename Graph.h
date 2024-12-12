#pragma once
#include "Eigen/Dense"

enum MatType {
    ADJ,
    LAP
};

class Graph {
public:
    Graph();
    Graph(int initialSize);                     // Fills with 0s
    ~Graph();
    void addVertex(float x, float y);
    void addVertex(float x, float y, int* connections);
    void removeVertex(int index);
    void addEdge(int v1, int v2);
    void removeEdge(int v1, int v2);
    void print();
    void updateUIGraph();
    int getSize();
    int** getAdjMat();
    int** getLapMat();
    int** getMatByType(MatType matType);
    void updateLapMat();
    int getEdgeCount();
    void updateEdgeCount();
    void updateEigen();
    int* getSmallestEigIndices();
    void updateEigenCoords();
    float** getEigenCoords();
    void update();
    int getMaxDegree();
    int getMinDegree();
    
protected:
    int** adjMat;
    int** lapMat;
    int size;
    int edgeCount;
    Eigen::VectorXf* eigenValues;
    Eigen::MatrixXf* eigenVectors;
    float** eigenCoords;
};