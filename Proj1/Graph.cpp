//
// Created by Rodrigo Gonçalves Figueiredo on 06/03/2023.
//

#include "Graph.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <list>
#include <queue>
#include <unistd.h>
#include <map>


using namespace std;

void Graph::resetResiduals() {
    for (auto & node : vertexs) {
        for (auto & edge : node.getAdj()) {
            edge.setResidual(edge.getCapacity());
        }
    }
}

void Graph::bfsConnectedPaths(int source) {
    for (auto & node : vertexs) {
        node.setVisited(false);
        node.setDist(-1);
    }
    queue<int> q; // queue of unvisited vertexes
    q.push(source);
    vertexs[source].setVisited(true);
    vertexs[source].setDist(0);
    while (!q.empty()) { // while there are still unvisited vertexes
        int u = q.front(); q.pop();
        for (auto& e : vertexs[u].getAdj()) {
            int w = e.getPointB();
            if (!vertexs[w].getVisited()) {
                q.push(w);
                vertexs[w].setVisited(true);
                vertexs[w].setDist(vertexs[u].getDist()+1);
            }
        }
    }
}


int Graph::bfsMaxFlow(int source, int dest, vector<int>& parent){
    //cout << "tou na bfs\n";
    for (auto & node : vertexs) {
        node.setVisited(false);
        node.setDist(-1);
    }
    queue<int> q; // queue of unvisited vertexes
    q.push(source);
    parent[source] = 0;
    vertexs[source].setVisited(true);
    while (!q.empty()) { // while there are still unvisited vertexes
        int u = q.front(); q.pop();
        for (auto& e : vertexs[u].adj) {
            int w = e.getPointB();
            if (!vertexs[w].getVisited() && e.getResidualCapacity() > 0 && e.isOK()){
                q.push(w);
                vertexs[w].setVisited(true);
                //cout << "Residual: " << e.getResidualCapacity() << endl;
                parent[w] = u;
                //std::cout << "Point B: " << vertexs[e.getPointB()].getName() << endl;
                if(w == dest){
                    //std::cout << "destino found\n";
                    return 0;
                }
            }
        }
    }
    return -1;
}

bool Graph::pathIsPossible(int source, int dest) {
    bfsConnectedPaths(source);
    return vertexs[dest].getVisited();
}

bool Graph::addVertex(Station_Vertex& station) {
    for(const Station_Vertex& st : vertexs)
        if(st.getName() == station.getName())
            return false;
    vertexs.push_back(station);
    return true;
}

bool Graph::addEdge(int source, int dest, Path_Edge& edge) {
    if (source<0 || source > (vertexs.size()-1) || dest<0 || dest>(vertexs.size()-1)) return false;
    vertexs[source].adj.push_back(edge);
    return true;
}


vector<Station_Vertex>& Graph::getVertexs() {return vertexs;}


