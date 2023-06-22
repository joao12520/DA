/**
 * @file Gestor.h
 * @brief Contains the implementation of the Gestor class and related functions.
 */


#ifndef DAPROJ2_GESTOR_H
#define DAPROJ2_GESTOR_H

#include <string>
#include "Graph.h"
#include <fstream>
#include <iostream>

using namespace std;


/**
 * @class Gestor
 * @brief The Gestor class provides functions to manage graphs and perform operations on them.
 */
class Gestor {
    public:
        /**
         * @brief Calculates the total weight of a given path in a graph.
         * @param g The graph.
         * @param path The path represented by a vector of node IDs.
         * @return The total weight of the given path.
         */
        double finalWeight(Graph g, vector<int> path);

        /**
         * @brief Reads a small network from a file and constructs a graph.
         * @param filename The name of the file containing the network data.
         * @return The constructed graph.
         */
        Graph readSmallNetwork(const string& filename);

        /**
        * @brief Reads nodes from a file and constructs a graph.
        * @param filename The name of the file containing the node data.
        * @return The constructed graph.
        */
        Graph readNodes(const string& filename);

        /**
         * @brief Reads edges from a file and adds them to the given graph.
         * @param filename The name of the file containing the edge data.
         * @param graph The graph to add the edges to.
         * @return The modified graph with the added edges.
         */
        Graph readEdges(const string& filename, Graph graph);

        /**
         * @brief Creates a random graph with the specified number of nodes.
         * @param n The number of nodes in the graph.
         * @return The randomly generated graph.
         */
        Graph createGraph(int n);
};


#endif