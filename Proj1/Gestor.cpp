//
// Created by Rodrigo Gonçalves Figueiredo on 06/03/2023.
//
#include "Gestor.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "Station_Vertex.h"
#include "map"

using namespace std;

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

void Gestor::readStations(const string& stationsfilename) {
    bool first = true;
    ifstream ficheiro_estacoes(stationsfilename);
    int count = 0;
    for(string line; getline(ficheiro_estacoes,line);){
        if (first){
            first = false;
            continue;
        }
        vector<string> parametros = parseString(line,',');
        Station_Vertex estacao = Station_Vertex(parametros[0], parametros[1], parametros[2],
                                                parametros[3], parametros[4], count);
        if(stations.find(estacao) == stations.end()){
            stations.insert(estacao);
            network.addVertex(estacao);
            count++;
        }
    }
}

void Gestor::readNetwork(const string& networkfilename) {
    bool first = true;
    ifstream ficheiro_network(networkfilename);
    for(string line; getline(ficheiro_network,line);){
        if (first){
            first = false;
            continue;
        }
        vector<string> parametros = parseString(line,',');
        if(stations.find(Station_Vertex(parametros[0])) != stations.end()
           and stations.find(Station_Vertex(parametros[1])) != stations.end()){
            Path_Edge edge1 = Path_Edge(stations.find(Station_Vertex(parametros[0]))->getCode()
                    , stations.find(Station_Vertex(parametros[1]))->getCode(),
                                        stoi(parametros[2]),parametros[3]);
            Path_Edge edge2 = Path_Edge(stations.find(Station_Vertex(parametros[1]))->getCode()
                    , stations.find(Station_Vertex(parametros[0]))->getCode(),
                                        stoi(parametros[2]),parametros[3]);
            network.addEdge(edge1.getPointA(),edge1.getPointB(),edge1);
            network.addEdge(edge2.getPointA(),edge2.getPointB(), edge2);
        }
    }
}

bool Gestor::pathIsOk(string source, string dest) {
    if(stations.find(Station_Vertex(source)) != stations.end()
       and stations.find(Station_Vertex(dest)) != stations.end()){
        return network.pathIsPossible(stations.find(Station_Vertex(source))->getCode(),
                                      stations.find(Station_Vertex(dest))->getCode());
    }
    return false;
}

void Gestor::assertFlow(int flow, int parent, int child) {
    //cout << "flow: " << flow <<  endl;
    //cout << "2 while" << endl;
    for(auto& edge : network.vertexs[parent].adj){
        if(edge.getPointB() == child){
            edge.setResidual(edge.getResidualCapacity() - flow);
            edge.setFlow(flow);
            //cout << "1 for"<< endl;
            break;
        }
    }
    for(auto& edge : network.vertexs[child].adj){
        if(edge.getPointB() == parent){
            edge.setResidual(edge.getResidualCapacity() + flow);
            edge.setFlow(-flow);
            //cout << "2 for"<< endl;
            break;
        }
    }
    //cout << "assert terminou"<< endl;
}

void Gestor::setOK(){
    for(auto& station : network.vertexs){
        for(auto& edge : station.adj){
            edge.OK = true;
        }
    }
}

int Gestor::reliabilityCheck(vector<pair<string,string>> edgesToCut){
    string source, dest;
    for(auto& edge : edgesToCut){
        source = edge.first;
        dest = edge.second;
        int sourceCode = -1; int destCode = -1;
        if(stations.find(Station_Vertex(source)) != stations.end()
           and stations.find(Station_Vertex(dest)) != stations.end()) {
            sourceCode = stations.find(Station_Vertex(source))->getCode();
            destCode = stations.find(Station_Vertex(dest))->getCode();
            //cout << "Estações Válidas on relCheck" << endl;
        }
        if(sourceCode == -1 or destCode == -1) return -1;
        for(auto& node : network.getVertexs()){
            for(auto& edge : node.adj){
                if((edge.getPointA() == sourceCode and edge.getPointB() == destCode) || (edge.getPointA() == destCode and edge.getPointB() == sourceCode)){
                    //cout << "Aresta posta a falso\n" << endl;
                    edge.OK = false;
                }
            }
        }
    }
    return 0;
}

int Gestor::edmondsKarp(string src, string dst) {
    if(!pathIsOk(src,dst)) return 0;
    if(src == dst) return 0;
    int source = -1; int dest = -1;
    if(stations.find(Station_Vertex(src)) != stations.end()
       and stations.find(Station_Vertex(dst)) != stations.end()) {
        source = stations.find(Station_Vertex(src))->getCode();
        dest = stations.find(Station_Vertex(dst))->getCode();
        //cout << "Estações Válidas" << endl;
    }
    int maxFlow = 0;
    int flow;
    if(source == -1 or dest == -1) return 0;
    vector<int> parent(network.getVertexs().size(), -1);
    while(true){
        flow = INT16_MAX;
        //cout << "1 While" << endl;
        if(network.bfsMaxFlow(source, dest, parent) != 0){
            break;
        }
        for(int i = dest; i != source; i = parent[i]){
            //cout << "parent: " << parent[i] << " i: " << i << endl;
            for(auto& edge : network.vertexs[parent[i]].adj){
                if(edge.getPointB() == i){
                    if(edge.getResidualCapacity() < flow){
                        flow = edge.getResidualCapacity();
                    }
                    break;
                }
            }
        }
        for(int i = dest; i != source; i = parent[i]){
            assertFlow(flow, parent[i], i);
        }
        maxFlow += flow;
    }
    return maxFlow;
}

int Gestor::assertFlowWCost(int flow, int parent, int child, int cost) {
    //cout << "flow: " << flow <<  endl;
    //cout << "2 while" << endl;
    for(auto& edge : network.vertexs[parent].adj){
        if(edge.getPointB() == child){
            edge.setResidual(edge.getResidualCapacity() - flow);
            edge.setFlow(edge.getFlow() + flow);
            if(edge.getService() == "ALFA PENDULAR\r"){
                //cout << "ALFA PENDULAR" << endl;
                //cout << "[flow]: " << flow << endl;
                cost+=4*flow;
            }
            else if(edge.getService() == "STANDARD\r"){
                //cout << "STANDARD" << endl;
                cost+=2*flow;
            }
            //cout << "1 for"<< endl;
            break;
        }
    }
    for(auto& edge : network.vertexs[child].adj){
        if(edge.getPointB() == parent){
            edge.setResidual(edge.getResidualCapacity() + flow);
            edge.setFlow(edge.getFlow()-flow);
            //cout << "2 for"<< endl;
            break;
        }
    }
    return cost;
    //cout << "assert terminou"<< endl;
}

pair<int,int> Gestor::edmondsKarpWCost(std::string src, std::string dst) {
    network.resetResiduals();
    int cost = 0;
    if(!pathIsOk(src,dst)){ return {-1,-1};cout << "path not ok\n";}
    if(src == dst) return {-1,-1};
    int source = -1; int dest = -1;
    if(stations.find(Station_Vertex(src)) != stations.end()
       and stations.find(Station_Vertex(dst)) != stations.end()) {
        source = stations.find(Station_Vertex(src))->getCode();
        dest = stations.find(Station_Vertex(dst))->getCode();
        //cout << "Estações Válidas" << endl;
    }
    int maxFlow = 0;
    int flow;
    if(source == -1 or dest == -1) return {-1,-1};
    vector<int> parent(network.getVertexs().size(), -1);
    while(true){
        flow = INT16_MAX;
        //cout << "1 While" << endl;
        if(network.bfsMaxFlow(source, dest, parent) != 0){
            break;
        }
        cout << "----------------------" << endl;
        for(int i = dest; i != source; i = parent[i]){
            //cout << "station: " << network.vertexs[i].getName() << endl;
            //cout << "parent: " << parent[i] << " i: " << i << endl;
            for(auto& edge : network.vertexs[parent[i]].adj){
                if(edge.getPointB() == i){
                    if(edge.getResidualCapacity() < flow){
                        flow = edge.getResidualCapacity();
                    }
                    break;
                }
            }
        }
        for(int i = dest; i != source; i = parent[i]){
            //cout << "parent: " << parent[i] << " i: " << i << endl;
            cost = assertFlowWCost(flow, parent[i], i, cost);
        }
        maxFlow += flow;
        //cout << "MaxFlow: " << maxFlow << endl;
    }
    return {cost,maxFlow};
}

bool sortbysec(pair<string, int> a, pair<string, int> b){
    return a.second > b.second;
}

void Gestor::topKDis(unsigned int k){
    map<std::string, int> mapTotalFlow;
    map<std::string, int> mapNumStations;
    int thisMaxFlow;
    for (const auto& stationFim :  this->network.vertexs ){
        for (const auto& stationInitial :  this->network.vertexs ){
            thisMaxFlow = edmondsKarp(stationInitial.getName(), stationFim.getName());
            if ( thisMaxFlow == -1){
                continue;
            }
            mapTotalFlow[stationFim.getDistrict()]+= thisMaxFlow;
        }
        mapNumStations[stationFim.getDistrict()]++;
        network.resetResiduals();
    }
    vector<pair<string, double>> auxVec;
    map<string, int>::iterator it;
    map<string, int>::iterator it2;
    for(it=mapTotalFlow.begin(); it!=mapTotalFlow.end(); ++it){
        pair<string, double> newPair;
        it2 = mapNumStations.find(it->first);
        newPair.first = it->first;
        newPair.second = it->second/it2->second;
        auxVec.push_back(newPair);
    }
    sort(auxVec.begin(),auxVec.end(), sortbysec);
    for(int i = 0; i< (k <= auxVec.size() ? k : auxVec.size());i++){
        cout << i+1 << " | " << auxVec[i].first << " - " << auxVec[i].second <<endl;
    }
}

void Gestor::topKMuni(unsigned int topk){
    map<std::string, int> mapTotalFlow;
    map<std::string, int> mapNumStations;
    for (const auto& stationFim :  this->network.vertexs ){
        for (const auto& stationInitial :  this->network.vertexs ){
            int thisMaxFlow = edmondsKarp(stationInitial.getName(), stationFim.getName());
            if ( thisMaxFlow == -1){
                continue;
            }
            mapTotalFlow[stationFim.getMunicipality()]+= thisMaxFlow;
        }
        mapNumStations[stationFim.getMunicipality()]++;
        network.resetResiduals();
    }
    vector<pair<string, double>> auxVec;
    map<string, int>::iterator it;
    map<string, int>::iterator it2;
    for(it=mapTotalFlow.begin(); it!=mapTotalFlow.end(); ++it){
        pair<string, double> newPair;
        it2 = mapNumStations.find(it->first);
        newPair.first = it->first;
        newPair.second = it->second/it2->second;
        auxVec.push_back(newPair);
    }
    sort(auxVec.begin(),auxVec.end(), sortbysec);
    int distance = topk <= auxVec.size() ? topk : auxVec.size();
    for(int i = 0; i< distance;i++){
        cout << i+1 << " | " << auxVec[i].first << endl;
    }
}

void Gestor::capStation(const string& station){
    int cap = 0;
    for (const auto& stationNew : this->network.vertexs){
        int thisMaxFlow = edmondsKarp(stationNew.getName(), station);
        if (thisMaxFlow == -1) continue;
        cap+=thisMaxFlow;
    }
    cout << station << " | " << cap << endl;
}

Graph& Gestor::getNetwork() {return network;}
tabHStation Gestor::getStations() {return stations;}


void Gestor::maxFlowAllPairs() {
    map<string, int> maxFlowMap;
    int maxFlow = -1;
    for (auto it1 = stations.begin(); it1 != stations.end(); it1++) {
        for (auto it2 = it1; it2 != stations.end(); it2++) {
            network.resetResiduals();
            if (it1 == it2) continue;
            int flow = edmondsKarp(it1->getName(), it2->getName());
            if (flow > 0) {
                string pair = it1->getName() + " ; " + it2->getName();
                maxFlowMap[pair] = flow;
                if (flow > maxFlow) {
                    maxFlow = flow;
                }
            }
        }
    }
    cout << "MaxFlow (" << maxFlow << "):" << endl;
    for (auto &it: maxFlowMap) {
        if (it.second == maxFlow) {
            cout << it.first << " (requires the most amount of trains)" << endl;
        }
    }
}


map<std::string, int> Gestor::flowPerStation() {
    map<std::string, int> flowPerStation;
    int flow;
    for (const auto &stationFim: this->network.vertexs) {
        for (const auto &stationInitial: this->network.vertexs) {
            flow = edmondsKarp(stationInitial.getName(), stationFim.getName());
            if (flow == -1) {
                continue;
            }
            flowPerStation[stationFim.getName()] += flow;
        }
        network.resetResiduals();
    }
    return flowPerStation;
}

map<std::string, int> Gestor::flowPerStationRel(vector<pair<string, string>> edgesToCut) {
    map<std::string, int> flowPerStation;
    this->reliabilityCheck(edgesToCut);
    int flow;
    for (const auto &stationFim: this->network.vertexs) {
        for (const auto &stationInitial: this->network.vertexs) {
            flow = edmondsKarp(stationInitial.getName(), stationFim.getName());
            if (flow == -1) {
                continue;
            }
            flowPerStation[stationFim.getName()] += flow;
        }
        network.resetResiduals();
    }
    return flowPerStation;
}

void Gestor::compareReliability(int k, vector<pair<string, string>> edgesToCut) {
    if (edgesToCut.empty()) {
        cout << "No edges cut, stations won't differ in flows" << endl;
        return;
    }
    map<std::string, int> flowPerStation = this->flowPerStation();
    map<std::string, int> flowPerStationRel = this->flowPerStationRel(edgesToCut);
    vector<pair<string, int>> flowDif;
    map<string, int>::iterator it;
    map<string, int>::iterator it2;
    for (it = flowPerStation.begin(); it != flowPerStation.end(); ++it) {
        pair<string, int> newPair;
        it2 = flowPerStationRel.find(it->first);
        newPair.first = it->first;
        newPair.second = (it->second) - (it2->second);
        flowDif.push_back(newPair);
    }
    sort(flowDif.begin(),flowDif.end(), sortbysec);
    cout << "When cutting this edges : " << endl;
    for (auto e : edgesToCut){
        cout << e.first << " - " << e.second << endl;
    }
    cout << "This will be the top " << k << " most afected stations." << endl;
    for(int i=0;i<(k <= flowDif.size() ? k : flowDif.size());i++){
        cout << i+1 << " | " << flowDif[i].first << endl;
    }
}
