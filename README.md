# Isperia
A graph theorist's sketchpad made for MATH 453. Written in C++ using raylib for graphics.
## Features
### Graph Display
Isperia displays the graph with vertices that can be moved around by the user while maintaining adjacencies. The backend for the graph uses an adjacency matrix to represent it, and the UI system handles things like the position and size of nodes, and drawing the edges between them.
### Moving Vertices
The user can add vertices by dragging them in the main section when the "select" tool is selected.
### Adding Vertices
The user can add vertices simply by clicking in the main section when the "add vertices" tool is selected. This will add a new row and column to the adjacency matrix.
### Add Edges
The user can add edges by clicking on one vertex and then clicking on another when the "add edge" tool is selected. For vertices with index `i` and `j` in the adjacency matrix, This will add a new edge to the adjacency matrix by setting `adjMat[i][j]` and `adjMat[j][i]` to 1.
### Remove Vertices
The user can remove vertices by clicking on them when the "remove vertices" tool is selected. This will also remove all edges that have the removed vertex as an endpoint.
### Remove Edges
The user can remove edges by clicking on them when the "remove edges" tool is selected.
### Adjacency Matrix Display
Displays the graph's adjacency matrix, which shows which vertices are joined together by having a 1 at `adjMat[i][j]` if vertices with index `i` and `j` are joined and 0 if not.
### Laplacian Matrix Display
Displays the graph's Laplacian matrix, which has the degree of vertices on the diagonal, -1 wherever two vertices are joined, and 0 everywhere else. We calculate the laplacian matrix by subtracting the adjacency matrix from the degree matrix.
### 2D Cartesian Projection using Eigenvectors
Another view of the graph, which uses the graph's eigenvectors as cartesian coordinates. This can provide useful representations of graphs, with aspects like minimal edge overlapping. The coordinates are calculated by finding the two smallest non-zero eigenvalues of the adjacency matrix, and then using their corresponding eigenvectors as 2d cartesian coordinates.
### Information on Vertices and Edges
The number of vertices and edges is displayed in the top left corner.
### Information on Degree of Vertices
The maximum and minimum degree of the graph is displayed in the top left corner and when an individual vertex is selected, it's degree is displayed in the bottom center.
### Information on Components
The number of components is displayed in the top left corner.
### Information on if the Graph is Bipartite
Whether the graph is bipartite is shown in the top left corner.
### Eigenvalue and Eigenvector Information
