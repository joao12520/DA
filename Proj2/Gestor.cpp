#include "Gestor.h"
#include <string>
#include "Graph.h"
#include <fstream>
#include <iostream>
#include <set>
#include <algorithm>
#include <random>

using namespace std;

/**
 * @brief Sorts nodes based on their ID in ascending order.
 * @param n1 First node.
 * @param n2 Second node.
 * @return True if the ID of n1 is less than the ID of n2, False otherwise.
 */
bool nodesSorting(node n1, node n2){
    return n1.id < n2.id;
}


/**
 * @brief Creates a random graph with n nodes and random edges.
 * @param n The number of nodes in the graph.
 * @return The randomly generated graph.
 */
Graph Gestor::createGraph(int n){
    uniform_real_distribution<double> unif(0, 10000);
    default_random_engine randomEngine(std::chrono::system_clock::now().time_since_epoch().count());
    Graph g(n);
    for(int i = 0; i < n; i++){
        g.nodes[i].id = i;
    }
    for(int i = 0; i < n; i++){
        for(int j = i+1; j < n; j++){
            g.addEdge(i,j,unif(randomEngine));
        }
    }

    g.adjustGraph(false);

    return g;
}

/**
 * @brief Calculates the total weight of the given order of nodes in the graph.
 * @param g The graph.
 * @param orderNodes The order of nodes to visit.
 * @return The total weight of the given order of nodes.
 */
double Gestor::finalWeight(Graph g, vector<int> orderNodes){
    vector<node> nodes = g.getNodes();
    double total = 0;
    int first,previous;
    bool firstNode = true;
    for(int s : orderNodes){
        if(firstNode){
            firstNode = false;
            first = s;
            previous = s;
            continue;
        }
        for(auto e : nodes[s].edges){
            if(e.dest == previous){
                total += e.weight;
            }
        }
        previous = s;
    }
    for(auto e : nodes[previous].edges){
        if(e.dest == first){
            total += e.weight;
        }
    }
    std::cout << "\nThe minimal distance travelled to visit all nodes is " << total << " (1.5x error range)." << endl;
    return 0.0;
}


/**
 * @brief Parses a string and splits it into substrings using the specified separator.
 * @param str The input string to parse.
 * @param separator The character used to separate substrings.
 * @return A vector of strings containing the parsed substrings.
 */
vector<string> parseString (string str, char separator){

    vector<string> strings;
    bool insideQuotes = false;

    int currIndex = 0, i = 0;
    int startIndex = 0, endIndex = 0;
    while (i <= (int) str.size()){
        if(str[i] == '"')
            insideQuotes = !insideQuotes;
        if((str[i] == separator || i == (int) str.size()) && !insideQuotes) {
            endIndex = i;
            string subStr = "";
            subStr.append(str, startIndex, endIndex - startIndex);
            strings.push_back(subStr);
            currIndex += 1;
            startIndex = endIndex + 1;
        }
        i++;
    }
    return strings;
}

/**
 * @brief Reads a small network from a file and constructs a graph.
 * @param filename The name of the file containing the network data.
 * @return The constructed graph.
 */
Graph Gestor::readSmallNetwork(const string& filename){
    bool first = true;
    ifstream ficheiro(filename);
    vector<node> nodes;
    set<int> checkNode;
    node nodeVazio;
    for(string line; getline(ficheiro,line);){
        if (first){
            first = false;
            continue;
        }
        vector<string> parametros = parseString(line,',');
        int origem = stoi(parametros[0]);
        int destino = stoi(parametros[1]);
        double distancia = stod(parametros[2]);
        int size = nodes.size();
        if(checkNode.find(origem) == checkNode.end() ){
            while(size < origem +1 ){
                nodes.push_back(nodeVazio);
                size+=1;
            }
            checkNode.insert(origem);
            node nodeNovo;
            vector<edge> edges;
            nodeNovo.id = origem;
            nodeNovo.lat = 0;
            nodeNovo.longi = 0;
            nodeNovo.edges = edges;
            nodeNovo.visited = false;
            nodeNovo.pair = -1;
            nodeNovo.paired = false;
            nodeNovo.parent = origem;
            nodeNovo.isOdd = false;
            nodeNovo.isInBlossom = false;
            nodeNovo.base = origem;
            nodes[origem] = nodeNovo;
        }
        if(checkNode.find(destino) == checkNode.end() ){
            while(size < destino+1  ){
                nodes.push_back(nodeVazio);
                size+=1;
            }
            checkNode.insert(destino);
            node nodeNovo;
            vector<edge> edges;
            nodeNovo.id = destino;
            nodeNovo.lat = 0;
            nodeNovo.longi = 0;
            nodeNovo.edges = edges;
            nodeNovo.visited = false;
            nodeNovo.pair = -1;
            nodeNovo.paired = false;
            nodeNovo.parent = origem;
            nodeNovo.isOdd = false;
            nodeNovo.isInBlossom = false;
            nodeNovo.base = origem;
            nodes[destino] = nodeNovo;
        }
        edge edgeNovo;
        edgeNovo.source = origem;
        edgeNovo.dest = destino;
        edgeNovo.weight = distancia;
        nodes[origem].edges.push_back(edgeNovo);
        edge edgeNovo2;
        edgeNovo2.source = destino;
        edgeNovo2.dest = origem;
        edgeNovo2.weight = distancia;
        nodes[destino].edges.push_back(edgeNovo2);
    }
    sort(nodes.begin(), nodes.end(),nodesSorting);
    Graph grafo = Graph(nodes);
    return grafo;
}

/**
 * @brief Reads nodes from a file and constructs a graph.
 * @param filename The name of the file containing the node data.
 * @return The constructed graph.
 */
Graph Gestor::readNodes(const string& filename) {
    bool first = true;
    vector<node> nodes;
    vector<edge> edgesVazio;
    ifstream ficheiro(filename);
    for(string line; getline(ficheiro,line);){
        if (first){
            first = false;
            continue;
        }
        vector<string> parametros = parseString(line,',');
        node novoNode;
        novoNode.id = stoi(parametros[0]);
        novoNode.longi = stod(parametros[1]);
        novoNode.lat = stod(parametros[2]);
        novoNode.edges = edgesVazio;
        novoNode.visited = false;
        novoNode.pair = -1;
        novoNode.paired = false;
        novoNode.parent = -1;
        novoNode.isOdd = false;
        novoNode.isInBlossom = false;
        novoNode.base = novoNode.id;
        nodes.push_back(novoNode);
    }
    return Graph(nodes);
}

/**
 * @brief Reads edges from a file and adds them to the given graph.
 * @param filename The name of the file containing the edge data.
 * @param graph The graph to add the edges to.
 * @return The modified graph with the added edges.
 */
Graph Gestor::readEdges(const string& filename, Graph graph) {
    vector<node> nodes = graph.getNodes();
    bool first = true;
    ifstream ficheiro(filename);
    int count = 0;
    for(string line; getline(ficheiro,line);){
        if (first){
            first = false;
            continue;
        }
        vector<string> parametros = parseString(line,',');
        edge novoEdge;
        novoEdge.source = stoi(parametros[0]);
        novoEdge.dest = stoi(parametros[1]);
        novoEdge.weight = stod(parametros[2]);
        nodes[novoEdge.source].edges.push_back(novoEdge);
    }
    return Graph(nodes);
}


