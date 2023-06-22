cmake_minimum_required(VERSION 3.23.2)
project(projDA)

set(CMAKE_CXX_STANDARD 14)

add_executable(projDA
        Gestor.cpp Gestor.h Station_Vertex.h Graph.h Station_Vertex.cpp Path_Edge.h Path_Edge.cpp Graph.cpp main.cpp)
