/**

@file Gestor.h
@brief Contains the declaration of the Gestor class and its methods.
*/
#ifndef PROJDA_GESTOR_H
#define PROJDA_GESTOR_H

#include <string>
#include <unordered_set>
#include <vector>
#include "Station_Vertex.h"
#include "Graph.h"
#include <map>

using namespace std;

/**

@brief Hash function for the Station_Vertex structure.
This struct defines the hash function for the Station_Vertex structure used in the tabHStation hash table.
*/
struct StationHash {
// TODO
    int operator() (const Station_Vertex& a) const {
        int sum = 0;
        string v = a.getName();
        for (char i : v) {
            sum += i;
        }
        return sum;
    }
};

/**
@brief Equality function for the Station_Vertex structure.
This struct defines the equality function for the Station_Vertex structure used in the tabHStation hash table.
*/
struct StationEquals {
// TODO
    bool operator() (const Station_Vertex& a1, const Station_Vertex& a2) const {
        return a1 == a2;
    }
};
typedef unordered_set<Station_Vertex, StationHash, StationEquals> tabHStation;

/**
@brief Class responsible for managing the railway stations and network.
*/
class Gestor {
    tabHStation stations; /**< Hash table that stores all the stations. */
    Graph network; /**< Graph that represents the railway network. */
    public:

    /**
    @brief Reads the stations from a file.
    @param stationsfilename Name of the file containing the station information.
    */
    void readStations(const string& stationsfilename);

    /**
    @brief Reads the railway network from a file.
    @param networkfilename Name of the file containing the railway network information.
    */
    void readNetwork(const string& networkfilename);

    /**
    @brief Returns the stations hash table.
    @return Stations hash table.
    */
    tabHStation getStations();

    /**
    @brief Returns the railway network.
    @return Railway network.
    */
    Graph& getNetwork();

    /**
    @brief Checks if a path is possible between two stations.
    @note Time complexity : O(|V + E|)
    @param source Name of the departure station.
    @param dest Name of the arrival station.
    @return true if there is a path between the stations, false otherwise.
    */
    bool pathIsOk(string source, string dest);

    /**
    @brief Runs the Edmonds-Karp algorithm to calculate the maximum flow between two stations.
    @note Time complexity : O(|VE^2|)
    @param source Name of the departure station.
    @param dest Name of the arrival station.
    @return Maximum flow value between the stations.
    */
    int edmondsKarp(string source, string dest);

    /**
    @brief Asserts that the calculated flow, parent and cost arrays for a given source and destination.
     @note Time complexity : O(|V + E|)
    @param flow Maximum flow value.
    @param parent Parent node of the maximum flow.
    @param source Name of the departure station.
    */
    void assertFlow(int flow, int parent, int source);

    /**
    * @brief Prints the top k stations with highest average distance to other stations
    * @note Time complexity : O(|VE^2|)
    * @param k The number of top stations to print
    */
    void topKDis(unsigned int k);

    /**
    * @brief Prints the top k municipalities with highest average distance to other stations within the same municipality
    * @note Time complexity : O(|VE^2|)
    * @param k The number of top municipalities to print
    */
    void topKMuni(unsigned  int k);

    /**
    * @brief Prints the maximum flow that can arrive at a given station considering all possible paths
    * @note Time complexity : O(|VE^2|)
    * @param station The name of the station to calculate the maximum flow
    */
    void capStation(const string& station);

    /**
    * @brief Calculates the maximum flow between a source and a destination and the minimum cost associated while maintaining the service
    * @note Time complexity : O(|V + E|)
    * @param source The name of the source station
    * @param dest The name of the destination station
    * @return A pair with the maximum flow value and the corresponding cost
    */
    pair<int,int> edmondsKarpWCost(string source, string dest);

    /**
    * @brief Calculates the maximum flow between all station pairs in the network and prints the result for each pair
    * @note Time complexity : O(|V^3 * E^2|)
    */
    void maxFlowAllPairs();

    /**
    * @brief Asserts that the calculated flow, parent and cost arrays for a given source and destination are correct
    * @note Time complexity : O(|V + E|)
    * @param flow The calculated flow value
    * @param parent The parent array of the path
    * @param child The index of the destination station in the graph adjacency list
    * @param cost The calculated cost value
    * @return The added cost of the path
    */
    int assertFlowWCost(int flow, int parent, int child, int cost);

    /**
    * @brief Compares the reliability of the network before and after cutting a set of edges
    * @note Time complexity : O(|V^3 * E^2|)
    * @param k The number of edges to cut
    * @param edgesToCut The edges to cut
    */
    void compareReliability(int k, vector<pair<string,string>> edgesToCut);

    /**
    * @brief Calculates the flow value for each station in the network
    * @note Time complexity : O(|V^3 * E^2|)
    * @return A map with each station name as key and its corresponding flow value
    */
    map<std::string, int> flowPerStation();

    /**
    * @brief Calculates the flow value for each station in the network after cutting a set of edges
    * @note Time complexity : O(|V^3 * E^2|)
    * @param edgesToCut The edges to cut
    * @return A map with each station name as key and its corresponding flow value
    */
    map<std::string, int> flowPerStationRel(vector<pair<string,string>> edgesToCut);

    /**
    * @brief Sets the given edges to unavailable
    * @note Time complexity : O(|V + E|)
    * @param edgesToCut The edges to cut
    * @return The reliability value
    */
    int reliabilityCheck(vector<pair<string,string>> edgesToCut);

    /**
    * @brief Sets all the edges in the network to be available
    * @note Time complexity : O(|V + E|)
    */
    void setOK();
};

#endif //PROJDA_GESTOR_H