/**

@file Graph.h
@brief This file contains the declaration of the Graph class and its methods, used for managing a graph of Station_Vertex and Path_Edge objects.
*/
#ifndef PROJDA_GRAPH_H
#define PROJDA_GRAPH_H

#include <vector>
#include <string>
#include "Station_Vertex.h"
#include "Path_Edge.h"

/**

@brief The Graph class represents a graph of Station_Vertex and Path_Edge objects.
*/
class Graph {
public:
    /**
    @brief A vector of Station_Vertex objects representing the vertices of the graph.
    */
    vector<Station_Vertex> vertexs;

    /**
    @brief Adds a Station_Vertex to the graph.
    @param station The Station_Vertex to be added to the graph.
    @return true if the vertex was added successfully, false if it already exists in the graph.
    */
    bool addVertex(Station_Vertex& station);

    /**
    @brief Adds a Path_Edge to the graph.
    @param source The index of the source vertex of the edge.
    @param dest The index of the destination vertex of the edge.
    @param edge The Path_Edge to be added to the graph.
    @return true if the edge was added successfully, false if the indices are out of range.
    */
    bool addEdge(int source, int dest, Path_Edge& edge);

    /**
    @brief Gets the vector of Station_Vertex objects representing the vertices of the graph.
    @return The vector of Station_Vertex objects representing the vertices of the graph.
    */
    vector<Station_Vertex>& getVertexs();

    /**
    @brief Performs a breadth-first search to find all connected paths from a source vertex.
    @note Time complexity : O(|V + E|)
    @param code The index of the source vertex.
    */
    void bfsConnectedPaths(int code);

    /**
    @brief Performs a breadth-first search between a source vertex and a destination vertex and finds an augmenting path.
    @note Time complexity : O(|V + E|)
    @param source The index of the source vertex.
    @param dest The index of the destination vertex.
    @param parent A vector to store the parent of each vertex in the path found.
    @return 0 if a path was found, -1 if no path was found.
    */
    int bfsMaxFlow(int source, int dest, vector<int>& parent);

    /**
    @brief Checks if a path exists between a source vertex and a destination vertex.
    @note Time complexity : O(|V + E|)
    @param source The index of the source vertex.
    @param dest The index of the destination vertex.
    @return true if a path exists between the two vertices, false otherwise.
    */
    bool pathIsPossible(int source, int dest);

    /**
    @note Time complexity : O(|V + E|)
    @brief Resets the residual capacity of all edges in the graph to its original capacity.
    */
    void resetResiduals();
};


#endif //PROJDA_GRAPH_H
