/**
@file Graph.h
@brief Defines the Graph class and related data structures and functions.
*/

#ifndef DAPROJ2_GRAPH_H
#define DAPROJ2_GRAPH_H

#include <vector>
#include <set>
#include <queue>

using namespace std;


/**
@brief Represents an edge in the graph.
*/
struct edge{
    int source; /** The source node of the edge. */
    int dest; /** The destination node of the edge. */
    double weight; /** The weight of the edge. */

    /**
    @brief Checks if two edges are equal.
    @param e The edge to compare with.
    @return True if the edges are equal, False otherwise.
    */
    bool operator==(const edge& e) const {
        return ((this->source == e.source && this->dest == e.dest && this->weight == e.weight) || (this->source == e.dest && this->dest == e.source && this->weight == e.weight));
    }
};

/**
@brief Represents a node in the graph.
*/
struct node{
    int id; /** The ID of the node. */
    double lat; /** The latitude of the node. */
    double longi; /** The longitude of the node. */
    bool visited; /** Flag indicating if the node has been visited. */
    int base; /** The base node of the blossom. */
    bool paired; /** Flag indicating if the node is paired. */
    int pair; /** The paired node. */
    int parent; /** The parent node. */
    bool isOdd; /** Flag indicating if the node has odd degree. */
    bool isInBlossom; /** Flag indicating if the node is in a blossom. */
    vector<edge> edges; /** The edges connected to the node. */
};

/**
@brief Represents a graph.
*/
class Graph {
    int V; /** The number of nodes in the graph. */
    vector<int> currentPath; /** Stores the current path being explored. */
    vector<int> bestPath; /** Stores the shortest path found so far. */
    double bestPathLength; /** The length of the best path found so far. */

public:

    vector<node> nodes; /**< The nodes in the graph. */

    /**
     * @brief Default constructor for the Graph class.
     */
    Graph(){
        this->nodes = vector<node>();
    }

    /**
     * @brief Parameterized constructor for the Graph class.
     * @param n The number of nodes in the graph.
     */
    Graph(int n){
        this->nodes = vector<node>(n,{-1,-1,-1,false,-1,false,-1,-1,false,false,{}});
    }

    /**
    * @brief Parameterized constructor for the Graph class.
    * @param nodes The nodes in the graph.
    */
    Graph(vector<node> nodes) {
        this->nodes = nodes;
        this->V = nodes.size();
    };

    /**
     * @brief Retrieves the nodes in the graph.
     * @return The nodes in the graph.
     */
    vector<node> getNodes();

    /**
     * @brief Solves the Traveling Salesman Problem using the backtracking approach.
     * @param n The number of nodes.
     * @param path The array to store the path.
     * @return The length of the shortest path.
     */
    unsigned int tspBT(unsigned int n, unsigned int path[]);

    /**
     * @brief Recursive function for the backtracking approach to solve the Traveling Salesman Problem.
     * @param n The number of nodes.
     * @param curIndex The current index in the path.
     * @param curDist The current distance.
     * @param curPath The current path.
     * @param minDist The minimum distance.
     * @param path The array to store the path.
     */
    void tspBTRec(unsigned int n, unsigned int curIndex, unsigned int curDist, unsigned int curPath[], unsigned int &minDist, unsigned int path[]);

    /**
     * @brief Adds an edge to the graph.
     * @param source The source node of the edge.
     * @param dest The destination node of the edge.
     * @param weight The weight of the edge.
     */
    void addEdge(int source, int dest, double weight);

    /**
     * @brief Adds a node to the graph.
     * @param n The node to add.
     */
    void addNode(const node& n);

    /**
     * @brief Adds a perfect matching to the graph.
     * @param edges The edges representing the perfect matching.
     */
    void addPerfectMatching(vector<edge> edges);

    /**
     * @brief Finds an Eulerian cycle in the graph.
     * @return The nodes in the Eulerian cycle.
     */
    vector<int> eulerianCycle();

    /**
     * @brief Finds an augmenting path in the graph using the Blossom Algorithm.
     * @param s The starting node.
     * @return The destination node of the augmenting path, or -1 if no augmenting path is found.
     */
    int getAugmentingPath(int s);

    /**
     * @brief Resets the values of the nodes in the graph.
     */
    void reset_values();

    /**
     * @brief Resets the visited flag and parent of the nodes in the graph.
     */
    void reset_visited();

    /**
     * @brief Creates a new graph from a minimum spanning tree.
     * @param mst The minimum spanning tree.
     * @return The new graph.
     */
    Graph makeMSTGraph(const vector<edge>& mst) const;

    /**
     * @brief Marks nodes in a blossom during the Blossom Algorithm.
     * @param lca The lowest common ancestor node.
     * @param s The starting node.
     */
    void blossomMark(int lca, int s);

    /**
     * @brief Contracts a blossom during the Blossom Algorithm.
     * @param s The starting node.
     * @param u The first node of the blossom.
     * @param v The second node of the blossom.
     * @param q The queue of unvisited nodes.
     */
    void blossomContract(int s, int u, int v, queue<int> &q);

    /**
     * @brief Finds the lowest common ancestor of two nodes.
     * @param root The root node.
     * @param u The first node.
     * @param v The second node.
     * @return The lowest common ancestor.
     */
    int findCA(int root, int u, int v);

    /**
     * @brief Finds a minimum spanning tree using Prim's algorithm.
     * @return The edges of the minimum spanning tree.
     */
    vector<edge> mstPrim();


    /**
     * @brief Changes the pairs of nodes during the Blossom Algorithm.
     * @param v The starting node.
     */
    void changePairs(int v);

    /**
     * @brief Identifies nodes with odd degrees in the graph.
     * @param nods The nodes in the graph.
     */
    void oddDegree(const vector<node>& nods);

    /**
     * @brief Executes the Blossom Algorithm.
     * @param mst The minimum spanning tree.
     * @return The edges of the perfect matching.
     */
    vector<edge> BlossomAlgorithm(vector<edge> mst);


    /**
     * @brief Executes the Christofides Algorithm to find an approximate solution to the Traveling Salesman Problem.
     * @return The approximate solution.
     */
    vector<int> ChristofidesAlgorithm();

    /**
     * @brief Solves the Traveling Salesman Problem using Triangular Approximation algorithm.
     */
    void tspApproximation();

    /**
     * @brief Calculates the Haversine distance between two nodes.
     * @param node1 The first node.
     * @param node2 The second node.
     * @return The Haversine distance between the nodes.
     */
    double getDistance(const node &node1, const node &node2);

    /**
     * @brief Adjusts the graph so that it respects the triangular inequality rule.
     * @param toComplete flag to know if the graph should be completed or just adjusted
     */
    void adjustGraph(bool toComplete);
};


#endif
