#pragma once

class Graph {
public:
    Graph();
    Graph(int initialSize);                     // Fills with 0s
    // Graph(UIGraph* graphUI);
    // Graph(UIGraph* graphUI, int initialSize);   // Fills with 0s
    ~Graph();
    void addVertex(float x, float y);
    void addVertex(float x, float y, int* connections);
    void removeVertex(int index);
    void addEdge(int v1, int v2);
    void removeEdge(int v1, int v2);
    // UIGraph* getUIGraph();
    // UIVertex* getUIVertex(int index);
    void print();
    void updateUIGraph();
    int getSize();
    int** getAdjMat();
    
protected:
    int** adjMat;
    int size;
    // UIGraph* graphUI;
};