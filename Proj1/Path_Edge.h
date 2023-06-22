/**
@file Path_Edge.h
@brief Defines the Path_Edge class, which represents an edge in a graph
*/
#ifndef PROJDA_PATH_EDGE_H
#define PROJDA_PATH_EDGE_H

#include <string>

using namespace std;

/**
@class Path_Edge
@brief Represents an edge in a graph
This class represents an edge in a graph. Each edge has a source point (pointA),
a destination point (pointB), a capacity (the maximum flow it can support),
a current flow, a residual capacity (the remaining capacity after some flow has
been passed through the edge), and a service string (which describes the service
provided by this edge).
*/
class Path_Edge {
    int pointA; /**< The source point of the edge */
    int pointB; /**< The destination point of the edge */
    int capacity; /**< The maximum flow that the edge can support */
    int flow; /**< The current flow of the edge */
    int residualCapacity; /**< The remaining capacity after some flow has been passed through the edge */
    string service; /**< The service provided by this edge */
    public:
        /**
        @brief Constructs a Path_Edge object with the specified parameters
        @param source The source point of the edge
        @param dest The destination point of the edge
        @param capacity The maximum flow that the edge can support
        @param service The service provided by this edge
        */
        Path_Edge(int source, int dest, int capacity, string service){
            this->pointA = source;
            this->pointB = dest;
            this->capacity = capacity;
            this->service = service;
            this->residualCapacity = capacity;
            this->OK = true;
            this->flow = 0;
        }

        bool OK;/**< A boolean flag that indicates if the edge is available to traverse or not */

        /**
        @brief Getter of the source point of the edge
        @return The source point of the edge
        */
        int getPointA(){return pointA;}

        /**
        @brief Getter of the destination point of the edge
        @return The destination point of the edge
        */
        int getPointB(){return pointB;}

        /**
        @brief Getter of the OK parameter associated with the edge
        @return The value of the OK parameter associated with the edge
        */
        bool isOK(){return OK;}

        /**
        @brief Getter of the capacity parameter
        @return The capacity associated with the edge
        */
        int getCapacity() {return capacity;}

        /**
        @brief Getter of the flow parameter
        @return The flow associated with the edge
        */
        int getFlow() {return flow;}

        /**
        @brief Getter of the residualCapacity parameter
        @return The residual capacity of the edgee
        */
        int getResidualCapacity() {return residualCapacity;}

        /**
        @brief Sets the parameter residualCapacity to the given value
        @param residual The new value of the residual capacity
        */
        void setResidual(int residual) {residualCapacity = residual;}

        /**
        @brief Sets the parameter flow to the given value
        @param flow The new value of the flow
        */
        void setFlow(int flow) {this->flow = flow;}

        /**
        @brief Getter function of the service string
        @return The associated service of the edge
        */
        string getService(){return service;}
};


#endif //PROJDA_PATH_EDGE_H
