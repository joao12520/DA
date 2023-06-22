//
// Created by Rodrigo Gonçalves Figueiredo on 06/03/2023.
//

#include "Station_Vertex.h"

string Station_Vertex::getName() const {return name;}
string Station_Vertex::getDistrict() const {return district;}
string Station_Vertex::getLine() const {return line;}
string Station_Vertex::getMunicipality() const {return municipality;}
string Station_Vertex::getTownship() const {return township;}
int Station_Vertex::getDist() const {return dist;}
int Station_Vertex::getVisited() const {return visited;}
void Station_Vertex::setVisited(bool vis){this->visited = vis;}
void Station_Vertex::setDist(int dist) {this->dist = dist;}
int Station_Vertex::getCode() const {return code;}
bool Station_Vertex::operator==(const Station_Vertex& s2) const{return this->getName() == s2.getName();}

