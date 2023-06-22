#include "Graph.h"
#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <queue>
#include <cmath>
#include <stack>

const double EARTH_RADIUS = 6371.0;  // Radius of the Earth in kilometers

/**
* @brief Adds an edge to the graph.
* @param source The source node ID of the edge.
* @param dest The destination node ID of the edge.
* @param weight The weight/cost of the edge.
*/
void Graph::addEdge(int source, int dest, double weight) {
    bool add = true;
    edge newEdge = {source, dest, weight};
    for (auto e: nodes[source].edges) {
        if (e.source == source && e.dest == dest && e.weight == weight) {
            add = false;
        }
    }
    if (add) {
        nodes[source].edges.push_back(newEdge);
        nodes[source].id = source;
    }
    add = true;
    edge newEdge2 = {dest, source, weight};
    for (auto e: nodes[dest].edges) {
        if (e.source == dest && e.dest == source && e.weight == weight) {
            add = false;
        }
    }
    if (add) {
        nodes[dest].edges.push_back(newEdge2);
        nodes[dest].id = dest;
    }

}

/**
* @brief Adds a node to the graph.
* @param n The node to be added.
*/
void Graph::addNode(const node &n) {
    nodes.push_back(n);
}

/**
* @brief Gets all the nodes in the graph.
* @return A vector of nodes in the graph.
*/
vector<node> Graph::getNodes() {
    return nodes;
}


//4.1. Backtracking

/**
@brief Solves the Traveling Salesman Problem (TSP) using backtracking. This function recursively finds the shortest Hamiltonian cycle (closed path) in a graph by exploring all possible paths and backtracking when a path becomes longer than the current minimum distance.
@param n The number of nodes in the graph.
@param curIndex The index of the current node being considered.
@param curDist The current distance traveled.
@param curPath An array representing the current path being constructed.
@param minDist The minimum distance found so far.
@param path An array to store the best path found.
*/
void Graph::tspBTRec(unsigned int n, unsigned int curIndex, unsigned int curDist, unsigned int curPath[],
                     unsigned int &minDist, unsigned int path[]) {
    double dist = 0.0;
    if (curIndex == n) {
        // add the distance back to the initial node

        for (edge e: nodes[curPath[n - 1]].edges) {
            if (e.dest == curPath[0]) {
                curDist += e.weight;
                break;
            }
        }
        //curDist += dists[curPath[n - 1]][curPath[0]];
        if (curDist < minDist) {
            minDist = curDist;
            // Copy the current state to the array storing the best state found so far
            for (unsigned int i = 0; i < n; i++) {
                path[i] = curPath[i];
            }
        }
        return;
    }
    // Try to move to the i-th vertex if the total distance does not exceed the best distance found and the vertex is not yet visited in curPath
    for (unsigned int i = 1; i <
                             n; i++) {// i starts at 1 and not 0 since it is assumed that node 0 is the initial node so it will not be in the middle of the path
        for (edge e: nodes[curPath[curIndex - 1]].edges) {
            if (e.dest == i) {
                dist = e.weight;
                break;
            }
        }
        if (curDist + dist < minDist) {
            bool isNewVertex = true;
            for (unsigned int j = 1; j < curIndex; j++) {
                if (curPath[j] == i) {
                    isNewVertex = false;
                    break;
                }
            }
            if (isNewVertex) {
                curPath[curIndex] = i;
                for (edge e: nodes[curPath[curIndex - 1]].edges) {
                    if (e.dest == curPath[curIndex]) {
                        dist = e.weight;
                        break;
                    }
                }
                tspBTRec(n, curIndex + 1, curDist + dist, curPath, minDist, path);
            }
        }
    }
}

/**
* @brief Finds the minimum distance Hamiltonian path using backtracking.
* @param n The number of nodes in the graph.
* @param path An array to store the best path found.
* @return The minimum distance of the Hamiltonian path.
*/
unsigned int Graph::tspBT(unsigned int n, unsigned int path[]) {
    unsigned int curPath[10000]; // static memory allocation is faster :)
    unsigned int minDist = std::numeric_limits<unsigned int>::max();

    // Assumes path starts at node 0 ...
    curPath[0] = 0;
    // ... so in the first recursive call curIndex starts at 1 rather than 0
    tspBTRec(n, 1, 0, curPath, minDist, path);

    cout << "Minimum distance: " << minDist << endl;
    for (unsigned int i = 0; i < n; i++) {
        cout << path[i] << " ";
    }
    return minDist;
}

//4.2.Triangular Approximation Heuristic

/**
@brief Calculates the distance between two nodes using the Haversine formula. The Haversine formula is used to compute the great-circle distance between two points on a sphere (e.g., Earth) given their latitude and longitude coordinates.
@param node1 The first node.
@param node2 The second node.
@return The distance between the two nodes in the same unit as the Earth's radius.
*/

double Graph::getDistance(const node &node1, const node &node2) {
    double lat1 = node1.lat;
    double lon1 = node1.longi;
    double lat2 = node2.lat;
    double lon2 = node2.longi;

    // Convert latitude and longitude from degrees to radians
    double lat1Rad = (lat1 * M_PI) / 180.0;
    double lon1Rad = (lon1 * M_PI) / 180.0;
    double lat2Rad = (lat2 * M_PI) / 180.0;
    double lon2Rad = (lon2 * M_PI) / 180.0;

    // Haversine formula to calculate the distance between two points on a sphere
    double dlon = lon2Rad - lon1Rad;
    double dlat = lat2Rad - lat1Rad;
    double a = pow(sin(dlat / 2.0), 2) + cos(lat1Rad) * cos(lat2Rad) * pow(sin(dlon / 2.0), 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    double distance = EARTH_RADIUS * c;

    return distance;
}

/**
* @brief Approximates the Travelling Salesman Problem (TSP) using a heuristic algorithm.
*/
void Graph::tspApproximation() {
    int numNodes = nodes.size();
    vector<bool> visited(numNodes, false);
    vector<int> path;
    int startNode = 0;
    double totalDistance = 0.0;

    visited[startNode] = true;
    path.push_back(startNode);

    while (path.size() < numNodes) {
        int currentNode = path.back();
        int nextNode = -1;
        double shortestDistance = numeric_limits<double>::max();

        for (const edge &e: nodes[currentNode].edges) {
            int neighbor = (e.source == currentNode) ? e.dest : e.source;
            double distance = e.weight;
            if (!visited[neighbor] && distance < shortestDistance) {
                shortestDistance = distance;
                nextNode = neighbor;
            }
        }

        if (nextNode != -1) {
            visited[nextNode] = true;
            path.push_back(nextNode);
            totalDistance += shortestDistance;
        } else {
            for (int i = 0; i < numNodes; i++) {
                if (!visited[i]) {
                    nextNode = i;
                    double distance = getDistance(nodes[currentNode], nodes[nextNode]);
                    if (distance < shortestDistance) {
                        shortestDistance = distance;
                    }
                }
            }
            visited[nextNode] = true;
            path.push_back(nextNode);
            totalDistance += shortestDistance;
        }
    }

    bool hasEdge = false;

    for (auto e: nodes[path[0]].edges)
        if (e.dest == path.back() && e.source == nodes[path[0]].id) {
            hasEdge = true;
        }

    // Connect back to the starting node
    if (!hasEdge) {
        totalDistance += getDistance(nodes[path.back()], nodes[startNode]);
    } else {
        for (auto e: nodes[path[0]].edges)
            if (e.dest == path.back() && e.source == nodes[path[0]].id) {
                totalDistance += e.weight;
            }
    }
    path.push_back(startNode);

    cout << "Approximate TSP Path: ";
    for (int node: path) {
        cout << node << " ";
    }
    cout << endl;
    cout << "Approximate TSP Path Length: " << totalDistance << endl;
}

//4.3. Christofides Heuristic

/**
@brief Finds the lowest common ancestor (LCA) of two nodes in the contracted graph.
This function finds the lowest common ancestor of two nodes in the contracted graph using the base and pair
information stored in the nodes. It starts from the given root node and iteratively follows the parent and base
pointers until it reaches the root node again.
@param root The root node of the contracted graph.
@param u The first node.
@param v The second node.
@return The lowest common ancestor of the two nodes in the contracted graph.
*/
int Graph::findCA(int root, int u, int v) {
    vector<int> used = {};
    while (true) {
        u = nodes[u].base;
        used.push_back(u);
        if (u == root)
            break;
        u = nodes[nodes[u].pair].parent;
    }
    while (true) {
        v = nodes[v].base;
        if (find(used.begin(), used.end(), v) != used.end()) {
            return v;
        } else {
            v = nodes[nodes[v].pair].parent;
        }
    }
}

/**
@brief Marks the nodes in a blossom.
This function marks the nodes in a blossom starting from the given LCA (lowest common ancestor) and going up
to the current node. It sets the isInBlossom flag to true for each node in the blossom. It also updates the parent
pointer for the current node if it is not the LCA.
@param lca The lowest common ancestor of the blossom.
@param s The current node.
*/
void Graph::blossomMark(int lca, int s) {
    while (nodes[s].base != lca) {
        int v = nodes[s].pair;
        nodes[nodes[s].base].isInBlossom = true;
        nodes[nodes[v].base].isInBlossom = true;
        s = nodes[v].parent;
        if (nodes[s].base != lca)
            nodes[s].parent = v;
    }
}

/**

@brief Contracts a blossom.
This function contracts a blossom in the graph. It marks the nodes in the blossom, updates the parent pointers,
and assigns the base node for each node in the blossom. It also adds the newly contracted nodes to the BFS queue
for further exploration.
@param s The source node of the current augmenting path.
@param u The first node of the blossom.
@param v The second node of the blossom.
@param q The queue of unvisited nodes.
*/
void Graph::blossomContract(int s, int u, int v, queue<int> &q) {
    for (auto &n: nodes) {
        n.isInBlossom = false;
    }
    int lca = findCA(s, u, v);
    blossomMark(lca, u);
    blossomMark(lca, v);
    if (nodes[u].base != lca)
        nodes[u].parent = v;
    if (nodes[v].base != lca)
        nodes[v].parent = u;

    for (auto u: nodes) {
        if (nodes[u.id].isInBlossom) {
            nodes[u.id].base = lca;
            if (!nodes[u.id].visited) {
                nodes[u.id].visited = true;
                q.push(u.id);
            }
        }
    }
}

//pode ser aqui o problema

/**
@brief Finds an augmenting path starting from the given source node.
This function performs a BFS search to find an augmenting path in the graph starting from the given source node.
It explores the graph by following the edges and updates the parent pointers for each visited node. If an augmenting
path is found, it returns the node where the path ends.
@param s The source node.
@return The node where the augmenting path ends, or -1 if no augmenting path is found.
*/
int Graph::getAugmentingPath(int s) {
    reset_visited();
    queue<int> q = {}; // queue of unvisited nodes
    q.push(s);
    nodes[s].visited = true;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (edge e: nodes[u].edges) {
            int v = e.dest;
            if (v != -1) {
                if (nodes[u].base != nodes[v].base && nodes[u].pair != v) {
                    if ((v == s) || (nodes[v].paired && nodes[nodes[v].pair].parent != -1)) {
                        blossomContract(s, u, v, q);
                    } else if (nodes[v].parent == -1) {
                        nodes[v].parent = u;
                        if (!nodes[v].paired)
                            return v;
                        else if (!nodes[v].visited) {
                            nodes[v].visited = true;
                            q.push(v);
                        }
                    }
                }
            }
        }
    }
    return -1;
}

/**
@brief Changes the pairs in the augmenting path.
This function changes the pairs in the augmenting path found by getAugmentingPath.
It iterates through the path, updating the pair and paired flags for each node.
@param v The node where the augmenting path ends.
*/
void Graph::changePairs(int v) {
    int currentNode = v, parentNode, pairNode;
    while (currentNode != -1) {
        parentNode = nodes[currentNode].parent;
        pairNode = nodes[parentNode].pair;
        nodes[parentNode].pair = currentNode;
        nodes[parentNode].paired = true;
        nodes[currentNode].pair = parentNode;
        nodes[currentNode].paired = true;
        currentNode = pairNode;
    }
}

/**
* @brief Finds the minimum weight matching of odd-degree nodes using the Blossom algorithm.
* @param mst A minimum spanning tree of the graph.
* @return A vector of edges representing the minimum weight matching.
*/
vector<edge> Graph::BlossomAlgorithm(vector<edge> mst) {
    int numNodes = nodes.size();
    vector<edge> matching;
    /*
    for(auto e : mst){
        cout << "edge-> Source: " << e.source << " Destiny: " << e.dest << " Weight: " << e.weight << endl;
    }
    */
    Graph g = Graph(nodes.size());

    int add = true;

    for (auto n: this->nodes) {
        if (n.isOdd) {
            for (edge e: n.edges) {
                if (nodes[e.dest].isOdd) {
                    add = true;
                    for (auto m: mst) {
                        if ((e.source == m.source && e.dest == m.dest && e.weight == m.weight) ||
                            (e.source == m.dest && e.dest == m.source && e.weight == m.weight)) {
                            add = false;
                        }
                    }
                    if (add) {
                        g.addEdge(e.source, e.dest, e.weight);
                    }
                }
            }
        }
    }
    /*
    for(auto n : g.nodes){
        cout << "node: " << n.id << " paired with " << n.pair << endl;
    }
    */
    g.reset_values();

    int furthestNode = -1;

    for (node &n: g.nodes) {
        if (!n.paired && n.id != -1) {
            //std::cout << "encontrei o odd: " << n.id << " " << n.paired << endl;
            furthestNode = g.getAugmentingPath(n.id);
            //std::cout << "encontrei o furthest: " << furthestNode << endl;
            if (furthestNode != -1) {
                g.changePairs(furthestNode);
            }
            //cout << "depois do odd: " << n.id << " " << n.paired << endl;
        }
    }
    /*
    for(auto n : g.nodes){
        cout << "node: " << n.id << " paired with " << n.pair << endl;
    }
    */
    for (auto n: g.nodes) {
        if (n.paired && n.pair != -1) {
            for (edge e: n.edges) {
                if (e.dest == n.pair) {
                    matching.push_back({e.source, e.dest, e.weight});
                    break;
                }
            }
        }
    }

    return matching;
}

/**
@brief Resets the values of all nodes in the graph.
This function resets the visited, isInBlossom, paired, pair, base, and parent values of all nodes in the graph.
*/
void Graph::reset_values() {
    for (auto &n: this->nodes) {
        n.visited = false;
        n.isInBlossom = false;
        n.paired = false;
        n.pair = -1;
        n.base = n.id;
        n.parent = -1;
    }
}

/**
@brief Resets the visited, parent, and base values of all nodes in the graph.
*/
void Graph::reset_visited() {
    for (auto &n: this->nodes) {
        n.visited = false;
        n.parent = -1;
        n.base = n.id;
    }
}

/**
@brief Performs Prim's algorithm to find the minimum spanning tree (MST) of the graph.
This function uses Prim's algorithm to find the MST of the graph. It starts from node 0 as the initial node,
maintains a priority queue of edges, and keeps track of the parent and distance of each node. It returns the edges
of the MST.
@return The edges of the minimum spanning tree.
*/
vector<edge> Graph::mstPrim() {
    int numNodes = nodes.size();
    vector<int> parent(numNodes, -1);
    vector<double> dist(numNodes, std::numeric_limits<double>::max());
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>> pq;
    pq.emplace(0.0, 0);
    dist[0] = 0.0;
    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        nodes[u].visited = true;

        for (const edge &e: nodes[u].edges) {
            int v = (e.source == u) ? e.dest : e.source;
            double weight = e.weight;
            if (!nodes[v].visited && weight < dist[v]) {
                parent[v] = u;
                dist[v] = weight;
                pq.emplace(dist[v], v);
            }
        }
    }
    vector<edge> newEdges;
    for (int i = 1; i < numNodes; i++) {
        double weight;
        for (auto e: nodes[i].edges) {
            if (e.dest == parent[i]) {
                weight = e.weight;
                break;
            }
        }
        edge newEdge = {
                i,
                parent[i],
                weight};
        newEdges.push_back(newEdge);
    }
    return newEdges;
}

/**
@brief Determines if each node in the given vector has an odd number of edges.
This function checks if each node in the given vector has an odd number of edges. If a node has an odd number of edges,
its "isOdd" flag is set to true in the current graph.
@param nods The vector of nodes to check.
*/
void Graph::oddDegree(const vector<node> &nods) {
    for (node n: nods) {
        if (n.edges.size() % 2 == 1) {
            this->nodes[n.id].isOdd = true;
        }
    }
}

/**
@brief Adds the edges of a perfect matching to the graph.
This function adds the edges of a perfect matching to the graph. The edges are provided as a vector.
@param edges The edges of the perfect matching to add.
*/
void Graph::addPerfectMatching(vector<edge> edges) {
    for (auto e: edges) {
        this->nodes[e.source].edges.push_back(e);
    }
}

/**
@brief Finds an Eulerian cycle in the graph and then a Hamiltonian circuit.
This function finds an Eulerian cycle in the graph using the Hierholzer's algorithm. It starts from a given node,
explores all edges in a depth-first manner, and returns the Eulerian cycle as a vector of node IDs. Then, it cleans the repeated vertexes
and finds a Hamiltonian circuit.
@return The Eulerian cycle in the graph.
*/
vector<int> Graph::eulerianCycle() {
    bool add = true;
    vector<int> path;
    stack<int> stackNodes = {};
    int currentNode = 0;
    vector<edge> Used = {};
    for (const node &n: nodes) {
        stackNodes.push(n.id);
        break;
    }
    while (!stackNodes.empty()) {
        currentNode = stackNodes.top();
        path.push_back(stackNodes.top());
        stackNodes.pop();
        for (edge e: nodes[currentNode].edges) {
            add = true;
            for (edge u: Used) {
                if ((e.source == u.source && e.dest == u.dest && e.weight == u.weight) ||
                    (e.source == u.dest && e.dest == u.source && e.weight == u.weight)) {
                    add = false;
                    break;
                }
            }
            if (add) {
                Used.push_back(e);
                stackNodes.push(e.dest);
            }
        }
    }

    set<int> used = {};
    vector<int> new_path = {};
    for (int i: path) {
        if (used.find(i) == used.end()) {
            new_path.push_back(i);
            used.insert(i);
        }
    }

    return new_path;
}

/**
@brief Creates a new graph with the given edges as the minimum spanning tree.
This function creates a new graph with the given edges as the minimum spanning tree. It copies the nodes from the
current graph and adds the edges of the MST to the new graph.
@param mst The edges of the minimum spanning tree.
@return The new graph with the given MST edges.
*/
Graph Graph::makeMSTGraph(const vector<edge> &mst) const {
    Graph g = Graph(nodes.size());
    for (auto e: mst) {
        g.addEdge(e.source, e.dest, e.weight);
    }
    return g;
}

/**
@brief Implements the Christofides' algorithm to find an approximate solution to the Traveling Salesman Problem.
This function implements the Christofides' algorithm to find an approximate solution to the Traveling Salesman Problem.
It first resets the values of all nodes in the graph. Then, it performs Prim's algorithm to find the minimum spanning
tree (MST) of the graph and creates a new graph with the MST edges. It identifies nodes with odd degrees in the new
graph and applies the Blossom algorithm to find a perfect matching. The perfect matching edges are added to the new
graph, and an Eulerian cycle is found. Finally, an Hamiltoninan circuit is returned as the approximate solution to the TSP.
@return The approximate solution to the Traveling Salesman Problem.
*/
vector<int> Graph::ChristofidesAlgorithm() {
    this->reset_values();
    vector<node> path;
    vector<edge> mst = mstPrim();
    Graph g = makeMSTGraph(mst);
    this->oddDegree(g.getNodes());
    vector<edge> matching = this->BlossomAlgorithm(mst);
    g.addPerfectMatching(matching);
    //cout << "Perfect matching with mst graph" << endl;
    vector<int> final_path = g.eulerianCycle();
    return final_path;
}

/**
 * @brief This function adjusts the graph edges so that the triangular inequality is met.
 * The function iterates through the existing edges and checks if, in a pair of three vertexes (A, B and C), the weights of
 * the edges that connect them follow the rule of Weight(A->B) + Weight(B->C) >= Weight(A->C).
 * @param toComplete
 */
void Graph::adjustGraph(bool toComplete) {
    Graph g = Graph(nodes.size());
    int n = (int)nodes.size();
    auto** weights = new double*[n];

    for(int i = 0; i < n;i++){
        weights[i] = new double[n];
    }


    for(int i = 0; i < n;i++){
        for(int j = 0; j < n;j++){
            if(i == j){
                weights[i][j] = 0;
            }
            else{
                weights[i][j] = std::numeric_limits<double>::max();
            }
        }
    }

    for(auto n : nodes){
        for(auto e : n.edges){
            weights[n.id][e.dest] = e.weight;
        }
    }

    if(toComplete){
        for(int k = 0; k < n; k++){
            for(int i = 0; i < n; i++){
                for(int j = 0; j < n;j++){
                    if((weights[i][j] > weights[i][k] + weights[k][j]) && (weights[i][j]) == std::numeric_limits<double>::max()){
                        weights[i][j] = weights[i][k] + weights[k][j];
                    }
                }
            }
        }
    }else{
        for (int k = 0; k < n; k++) {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    if (weights[i][k] + weights[k][j] < weights[i][j]) {
                        weights[i][j] = weights[i][k] + weights[k][j];
                    }
                }
            }
        }
    }


    for(int i = 0; i < n;i++){
        for(int j = i + 1; j < n;j++){
            g.addEdge(i,j, weights[i][j]);
        }
    }


    this->nodes = g.nodes;

}


