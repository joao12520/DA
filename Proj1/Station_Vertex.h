/**
 * @file Station_Vertex.h
 * @brief Defines the Station_Vertex class.
 */
#ifndef PROJDA_STATION_VERTEX_H
#define PROJDA_STATION_VERTEX_H

#include <string>
#include "Path_Edge.h"
#include <vector>

using namespace std;

/**
 * @class Station_Vertex
 * @brief Represents a vertex in a graph that represents a station in a transportation network.
 */
class Station_Vertex {
    string name; ///< The name of the station.
    string district; ///< The district where the station is located.
    string municipality; ///< The municipality where the station is located.
    string township; ///< The township where the station is located.
    string line; ///< The line to which the station belongs.
    int code; ///< A code associated with the station vertex.
    int dist; ///< The distance of the station vertex from some other vertex in a graph.
    bool visited = false; ///< Indicates whether the station vertex has been visited in a graph traversal.
public:
    vector<Path_Edge> adj; ///< The adjacency list of the station vertex.

    /**
     * @brief Constructs a new Station_Vertex object.
     * @param name The name of the station.
     * @param district The district where the station is located.
     * @param municipality The municipality where the station is located.
     * @param township The township where the station is located.
     * @param line The line to which the station belongs.
     * @param code A code associated with the station vertex.
     */
    Station_Vertex(string name, string district = "null", string municipality = "null",string township = "null", string line = "null", int code = -1){
        this->name = name;
        this->district = district;
        this->municipality = municipality;
        this->township = township;
        this->line = line;
        this->code = code;
        this->dist = -1;
    };

    /**
     * @brief Gets the name of the station.
     * @return The name of the station.
     */
    string getName() const;

    /**
     * @brief Gets the district where the station is located.
     * @return The district where the station is located.
     */
    string getDistrict() const;

    /**
     * @brief Gets the municipality where the station is located.
     * @return The municipality where the station is located.
     */
    string getMunicipality() const;

    /**
     * @brief Gets the township where the station is located.
     * @return The township where the station is located.
     */
    string getTownship() const;

    /**
     * @brief Gets the line to which the station belongs.
     * @return The line to which the station belongs.
     */
    string getLine() const;

    /**
     * @brief Gets the code associated with the station vertex.
     * @return The code associated with the station vertex.
     */
    int getCode() const;

    /**
     * @brief Gets the adjacency list of the station vertex.
     * @return The adjacency list of the station vertex.
     */
    vector<Path_Edge>& getAdj() {return adj;}

    /**
     * @brief Overloads the equality operator to compare two Station_Vertex objects based on their name member variable.
     * @param s2 The Station_Vertex object to compare to this object.
     * @return true if the two Station_Vertex objects have the same name, false otherwise.
     */
    bool operator==(const Station_Vertex& s2) const;

    /**
     * @brief Sets the visited status of the station vertex.
     * @param vis The visited status of the station vertex.
     */
    void setVisited(bool vis);

    /**
    @brief Sets the distance from the source to this vertex.
    @param dist The distance from the source to this vertex.
    */
    void setDist(int dist);

    /**
    @brief Gets the value of the visited attribute.
    @return The value of the visited attribute.
    */
    int getVisited() const;

    /**
    @brief Gets the distance from the source to this vertex.
    @return The distance from the source to this vertex.
    */
    int getDist() const;
};


#endif //PROJDA_STATION_VERTEX_H
