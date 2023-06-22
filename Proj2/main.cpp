/**
 * @file main.cpp
 * @brief Main file for the TSP Solver program.
 */

#include "Gestor.h"
#include "Graph.h"
#include <string>
#include <chrono>
#include <unistd.h>
#include <filesystem>

#define NOT_VALID "This option is not valid! Try a valid one. \n"

using namespace std;

/**
 * @brief Main function for the TSP Solver program.
 * @return 0 on successful execution.
 */
int main(){
    
    Gestor ges;
    vector<node> nodes;
    std::__fs::filesystem::path pathToFolder = "../Project2Graphs";
    std::__fs::filesystem::path pathToFile;
    Graph g = Graph(nodes);
    char mainChoice = 0;
    char secondaryChoice;
    string auxChoice;
    char parseChoice = 0;

    while(mainChoice != '0') {

        cout << "\n-=-=-=-=-=-=-=-=-MENU-=-=-=-=-=-=-=-=-" << endl;
        cout << "| 1. Parse files to graphs           |" << endl;
        cout << "| 2. Create your won complete graph  |" << endl;
        cout << "| 3. Find best path                  |" << endl;
        cout << "| 4. List all nodes                  |" << endl;
        cout << "| 0. Quit                            |" << endl;
        cout << "-=-=-=-=-=-=-=-=--==--=-=-=-=-=-=-=-=-" << endl;
        cout << "[->] ";
        cin >> auxChoice;
        cout << endl;

        if (auxChoice.length() > 1) {
            cout << NOT_VALID;
            continue;
        }

        mainChoice = auxChoice[0];

        switch(mainChoice){
            case '1':
                cout << "\n-=-=-=-=-=-=-PARSE FILE-=-=-=-=-=-=-" << endl;
                cout << "| 0. Input file path               |" << endl;
                cout << "| 1. Graph 1 (1000 nodes)          |" << endl;
                cout << "| 2. Graph 2 (5000 nodes)          |" << endl;
                cout << "| 3. Graph 3 (10000 nodes)         |" << endl;
                cout << "| 4. Toy-Graph Shipping            |" << endl;
                cout << "| 5. Toy-Graph Stadiums            |" << endl;
                cout << "| 6. Toy-Graph Tourism             |" << endl;
                cout << "| 7. Extra 50                      |" << endl;
                cout << "| 8. Extra 500                     |" << endl;
                cout << "| 9. Extra 900                     |" << endl;
                cout << "-=-=-=-=-=-=-=-=--==--=-=-=-=-=-=-=-" << endl;
                cout << "[->] ";
                cin >> auxChoice;
                cout << endl;

                if (auxChoice.length() > 1) {
                    cout << NOT_VALID;
                    continue;
                }

                secondaryChoice = auxChoice[0];

                switch (secondaryChoice) {
                    case '0': {
                        char numFiles = 0;
                        cout << "\nDo you wish to parse one or two files? (1/2)\n";
                        cout << "[->] ";
                        cin >> auxChoice;
                        cout << endl;
                        parseChoice = '0';
                        if (auxChoice.length() > 1 || (auxChoice[0] != '1' && auxChoice[0] != '2')) {
                            cout << NOT_VALID;
                            continue;
                        }
                        numFiles = auxChoice[0];

                        switch (numFiles){
                            case '1': {
                                cout << "\nInput file path: ";
                                cin >> auxChoice;
                                cout << endl;
                                pathToFile = auxChoice;
                                if (pathToFile.empty()) {
                                    cout << NOT_VALID;
                                    continue;
                                }
                                g = ges.readSmallNetwork(pathToFile);
                                break;
                            }
                            case '2':{
                                cout << "\nInput nodes file path: ";
                                cin >> auxChoice;
                                cout << endl;
                                pathToFile = auxChoice;
                                if (pathToFile.empty()) {
                                    cout << NOT_VALID;
                                    continue;
                                }
                                g = ges.readNodes(pathToFile);
                                cout << "\nInput edges file path: ";
                                cin >> auxChoice;
                                cout << endl;
                                pathToFile = auxChoice;
                                if (pathToFile.empty()) {
                                    cout << NOT_VALID;
                                    continue;
                                }
                                g = ges.readEdges(pathToFile, g);
                                break;
                            }
                            default:
                                cout << NOT_VALID;
                                continue;
                        }
                        break;
                    }
                    case '1':
                        g = ges.readNodes("../Project2Graphs/Real-World-Graphs/graph1/nodes.csv");
                        g = ges.readEdges("../Project2Graphs/Real-World-Graphs/graph1/edges.csv", g);
                        parseChoice = '1';
                        break;
                    case '2':
                        cout << "\nReading Graph 2...\n";
                        g = ges.readNodes("../Project2Graphs/Real-World-Graphs/graph2/nodes.csv");
                        g = ges.readEdges("../Project2Graphs/Real-World-Graphs/graph2/edges.csv", g);
                        parseChoice = '2';
                        break;
                    case '3':
                        cout << "\nReading Graph 3...\n";
                        g = ges.readNodes("../Project2Graphs/Real-World-Graphs/graph3/nodes.csv");
                        g = ges.readEdges("../Project2Graphs/Real-World-Graphs/graph3/edges.csv", g);
                        parseChoice = '3';
                        break;
                    case '4':
                        cout << "\nReading Toy-Graph Shipping...\n";
                        g = ges.readSmallNetwork("../Project2Graphs/Toy-Graphs/shipping.csv");
                        parseChoice = '4';
                        break;
                    case '5':
                        cout << "\nReading Toy-Graph Stadiums...\n";
                        g = ges.readSmallNetwork("../Project2Graphs/Toy-Graphs/stadiums.csv");
                        parseChoice = '5';
                        break;
                    case '6':
                        cout << "\nReading Toy-Graph Tourism...\n";
                        g = ges.readSmallNetwork("../Project2Graphs/Toy-Graphs/tourism.csv");
                        parseChoice = '6';
                        break;
                    case '7':
                        cout << "\nReading Extra 50...\n";
                        g = ges.readSmallNetwork("../Project2Graphs/Extra_Fully_Connected_Graphs/edges_50.csv");
                        parseChoice = '7';
                        break;
                    case '8':
                        cout << "\nReading Extra 500...\n";
                        g = ges.readSmallNetwork("../Project2Graphs/Extra_Fully_Connected_Graphs/edges_500.csv");
                        parseChoice = '8';
                        break;
                    case '9':
                        cout << "\nReading Extra 900...\n";
                        g = ges.readSmallNetwork("../Project2Graphs/Extra_Fully_Connected_Graphs/edges_900.csv");
                        parseChoice = '9';
                        break;
                    default:
                        cout << NOT_VALID;
                        break;
                }
                break;
            case '2': {
                string n;
                cout << "\nNumber of nodes of the desired graph: ";
                cin >> n;

                bool validInput = true;
                for (char c : n) {
                    if (!std::isdigit(c)) {
                        validInput = false;
                        break;
                    }
                }
                if(!validInput){
                    cout << "\nInvalid input! Please try again.\n";
                    break;
                }

                int num = std::stoi(n);

                g = ges.createGraph(num);
                cout << "\nGraph created!\n";
                break;
            }
            case '3': {
                cout << "\n-=-=-=-=-=-=-FIND BEST PATH-=-=-=-=-=-=-" << endl;
                cout << "| 1. Backtracking                       |" << endl;
                cout << "| 2. Triangular Approximation Heuristic |" << endl;
                cout << "| 3. Christofides Heuristic             |" << endl;
                cout << "-=-=-=-=-=-=-=-=--==--=-=-=-=-=-=-=-=-=-" << endl;
                cout << "[->] ";
                cin >> auxChoice;
                cout << endl;

                if (auxChoice.length() > 1) {
                    cout << NOT_VALID;
                    continue;
                }

                secondaryChoice = auxChoice[0];

                switch (secondaryChoice) {
                    case '1': {
                        auto timeStart = chrono::high_resolution_clock::now();
                        unsigned int path[g.nodes.size()];
                        g.tspBT(g.nodes.size(),path);
                        cout << "\nPlease wait...\n\n";
                        auto timeEnd = chrono::high_resolution_clock::now();
                        auto time = chrono::duration_cast<chrono::milliseconds>(timeEnd - timeStart);
                        if(time.count() > 1000)
                            cout << "Time elapsed: ~ " << time.count()/1000 << "s " << time.count()%1000 << "ms"<< endl;
                        else
                            cout << "Time elapsed: ~ " << time.count() << " ms" << endl;
                        sleep(1);
                        break;
                    }
                    case '2': {
                        auto timeStart = chrono::high_resolution_clock::now();
                        g.tspApproximation();
                        parseChoice = '0';
                        cout << "\nPlease wait...\n\n";
                        auto timeEnd = chrono::high_resolution_clock::now();
                        auto time = chrono::duration_cast<chrono::milliseconds>(timeEnd - timeStart);
                        if(time.count() > 1000)
                            cout << "Time elapsed: ~ " << time.count()/1000 << "s " << time.count()%1000 << "ms"<< endl;
                        else
                            cout << "Time elapsed: ~ " << time.count() << " ms" << endl;
                        sleep(1);
                        break;
                    }
                    case '3': {
                        if(parseChoice == '1' || parseChoice == '2' || parseChoice == '3'){
                            cout << "Graph too big for this algorithm" << endl;
                            sleep(1);
                            break;
                        } else if(parseChoice == '4'){
                            cout << "Graph is incomplete and therefore will not work with this heuristic" << endl;
                            sleep(1);
                            break;
                        }
                        auto timeStart = chrono::high_resolution_clock::now();
                        int count = 0;
                        vector<int> christofides = g.ChristofidesAlgorithm();
                        cout << "\nPlease wait...\n\n";
                        cout << "Christofides Heuristic: ";
                        for(auto n : christofides){
                            count++;
                            cout << " > ";
                            cout << " [" << n << "] ";
                        }
                        ges.finalWeight(g, christofides);
                        cout << "\nnumber of nodes: " << count << endl;
                        auto timeEnd = chrono::high_resolution_clock::now();
                        auto time = chrono::duration_cast<chrono::milliseconds>(timeEnd - timeStart);
                        if(time.count() > 1000)
                            cout << "Time elapsed: ~ " << time.count()/1000 << "s " << time.count()%1000 << "ms"<< endl;
                        else
                            cout << "Time elapsed: ~ " << time.count() << " ms" << endl;
                        //cout << "Time elapsed: ~" << time.count() << " ms" << endl;
                        sleep(1);
                        break;
                    }
                    default:
                        cout << NOT_VALID;
                        break;
                }
                break;
            }
            case '4':
                nodes = g.getNodes();
                for(auto e : nodes){
                    cout << "Node: " << e.id << "|" << e.lat << "|" << e.longi << endl;
                    for(auto f : e.edges){
                        cout << "   Edge: " << f.source << "|" << f.dest << "|" << f.weight << endl;
                    }
                }
                break;
            case '0':
                break;
            default:
                cout << NOT_VALID;
                break;
        }
    }
    return 0;
}