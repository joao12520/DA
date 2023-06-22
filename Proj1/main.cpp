//
// Created by Rodrigo Gonçalves Figueiredo on 07/03/2023.
//

#include <iostream>
#include "Gestor.h"
#include "Station_Vertex.h"
#include <string>
#include <limits>

using namespace std;

int main(){

        Gestor ges;
        string auxChoice;
        char choice;

        ges.readStations("../stations.csv");
        ges.readNetwork("../network.csv");





    cout << "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n"
            "|  This program will allow you to inspect several aspects of a railroad network. Here you will be able to see          |\n"
            "|  the maximum number of trains that can simultaneously travel between two stations, considering the capacity          |\n"
            "|  of the railroads, the top-k municipalities and districts that have the most, in average, trains incoming,           |\n"
            "|  you will also be able to see which pairs of stations require the most amount of trains when taking full             |\n"
            "|  advantage of the railroads capacity. You will also be able to analyze the number of trains that can simultaneously  |\n"
            "|  arrive at a given station. Also, you will be able to analyze the minimum cost of allocating the maximum number of   |\n"
            "|  trains that can simultaneously travel between two specific stations. Finally, you will be able to deactivate        |\n"
            "|  one or more edges of the network and see how it affects the maximum number of trains that can simultaneously travel |\n"
            "|  in it.                                                                                                              |\n"
            "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n";

    do {
        // Display menu options
        cout << "\nMenu:" << endl;
        cout << "1. Reliability Check" << endl;
        cout << "2. Calculate the maximum number of trains that can simultaneously travel between two stations" << endl;
        cout << "3. Top-K municipalities with max average of number of trains incoming" << endl;
        cout << "4. Top-K districts with max average of number of trains incoming" << endl;
        cout << "5. Which station/s require most amount of trains" << endl;
        cout << "6. Calculate the maximum number of trains that can simultaneously arrive at a given station" << endl;
        cout << "7. Calculate the maximum amount of trains that can simultaneously travel between two specific stations with minimum cost" << endl;
        cout << "8. Inspect network" << endl;
        cout << "9. Reset network to its original values" << endl;
        cout << "0. Quit" << endl;
        cout << "[->] ";
        cin >> auxChoice;

        if(auxChoice.length() > 1) {
            cout << "\nInvalid input\n";
            continue;
        }
        choice = auxChoice[0];

        switch(choice) {
            case '1': {
                char choice;
                cout << "\nMenu:" << endl;
                cout << "1. Deactivate one or more railroads in the railroad network" << endl;
                cout << "2. Deactivate one or more railroads and analyze its consequences in the railroad network" << endl;
                cout << "0. Back" << endl;
                cout << "[->] ";
                cin >> auxChoice;

                if(auxChoice.length() > 1) {
                    cout << "\nInvalid input\n";
                    continue;
                }
                choice = auxChoice[0];

                switch (choice) {
                    case '1':{
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        vector<pair<string,string>> edgesToCut;
                        string source, dest;
                        char wantToContinue = 'y';
                        cout << "[Here, you can choose one or more edges that you want to deactivate]" << endl;
                        while(wantToContinue == 'y') {
                            cout << "Enter the source station: ";
                            getline(cin, source);
                            cout << "Enter the destination station: ";
                            getline(cin, dest);
                            edgesToCut.emplace_back(source,dest);
                            cout << "Do you wish to choose more edges? (y/n)" << endl;
                            cin >> wantToContinue;
                        }
                        int result = ges.reliabilityCheck(edgesToCut);
                        if(result != 0){
                            cout << "\nThe provided stations are not valid\n";
                        }
                        break;
                    }
                    case '2': {
                        cin.clear();
                        int k;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        vector<pair<string,string>> edgesToCut;
                        string source, dest;
                        char wantToContinue = 'y';
                        cout << "[Here, you can choose one or more edges that you want to deactivate]" << endl;
                        while(wantToContinue == 'y') {
                            cout << "Enter the source station: ";
                            getline(cin, source);
                            cout << "Enter the destination station: ";
                            getline(cin, dest);
                            edgesToCut.emplace_back(source,dest);
                            cout << "Do you wish to choose more edges? (y/n)" << endl;
                            cin >> wantToContinue;
                        }
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "How many results would you like to see (top-k)?" << endl;
                        cin >> k;
                        ges.compareReliability(k, edgesToCut);
                        break;
                    }
                }
                break;
            }
            case '2': {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                string source, dest;
                cout << "Enter the source station: ";
                getline(cin, source);
                cout << "Enter the destination station: ";
                getline(cin, dest);
                ges.getNetwork().resetResiduals();
                int maxFlow = ges.edmondsKarp(source, dest);
                if(maxFlow == -1 || maxFlow == 0) {
                    cout << "No flow exists between " << source << " and " << dest <<endl;
                } else {
                    cout << "Maximum flow between " << source << " and " << dest << " is " << maxFlow << endl;
                }
                break;
            }
            case '3': {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                int k;
                cout << "Enter top-k municipalities regarding transportation needs: "<<endl;
                cin >> k;
                cout << "Please wait..." << endl;
                ges.topKMuni(k);
                cout << "\nHere are presented the top-" << k << " municipalities regarding budget allocation and transportation needs" << endl;
                break;
            }
            case '4': {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                int k;
                cout << "Enter top-k districts regarding transportation needs: " << endl;
                cin >> k;
                cout << "Please wait..." << endl;
                ges.topKDis(k);
                cout << "\nHere are presented the top-" << k << " districts regarding budget allocation and transportation needs" << endl;
                break;
            }
            case '5': {
                cout << "Please wait..." << endl;
                ges.maxFlowAllPairs();
                break;
            }
            case '6': {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                string stationA;
                cout << "Enter the station: ";
                getline(cin, stationA);
                ges.capStation(stationA);
                break;
            }
            case '7': {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                string stationA;
                string stationB;
                cout << "Source station: ";
                getline(cin, stationA);
                cout << "Destination station: ";
                getline(cin, stationB);
                pair<int,int> result = ges.edmondsKarpWCost(stationA,stationB);
                cout << stationA << " | " << stationB << "  | Cost: " << result.first << "  | Flow: " << result.second <<endl;
                break;
            }
            case '8':{
                char choice;
                cout << "\nMenu" << endl;
                cout << "1. List all existing stations" << endl;
                cout << "2. List all stations of a specific line" << endl;
                cout << "3. List all stations of a specific municipality" << endl;
                cout << "4. List all stations of a specific district" << endl;
                cout << "5. List all stations and its connections" << endl;
                cout << "6. List all stations and its connections of a specific line" << endl;
                cout << "7. Check if there is a path between two stations" << endl;
                cout << "0. Back" << endl;
                cout << "[->] ";
                cin >> auxChoice;

                if(auxChoice.length() > 1) {
                    cout << "\nInvalid input\n";
                    continue;
                }
                choice = auxChoice[0];

                switch (choice) {
                    case '1':
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "|---------------------------------------------|" << endl;
                        for(auto & station : ges.getStations()) {
                            cout << station.getName() << " " << station.getDistrict() << " " << station.getLine() << endl;
                        }
                        cout << "|---------------------------------------------|" << endl;
                        break;
                    case '2': {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        bool noGood = true;
                        string line;
                        cout << "Enter the line: ";
                        getline(cin, line);
                        line = line.append("\r");
                        cout << "|---------------------------------------------|" << endl;
                        for(auto & station : ges.getStations()) {
                            if(station.getLine() == line) {
                                noGood = false;
                                cout << station.getName() << " " << station.getDistrict() << " " << station.getLine() << endl;
                            }
                        }
                        cout << "|---------------------------------------------|" << endl;
                        if(noGood) {
                            cout << "Invalid line: " << line << endl;
                        }
                        break;
                    }
                    case '3': {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        bool noGood = true;
                        string line;
                        cout << "Enter the Municipality: ";
                        getline(cin, line);
                        cout << "|---------------------------------------------|" << endl;
                        for (auto &station: ges.getStations()) {
                            if (station.getMunicipality() == line) {
                                noGood = false;
                                cout << station.getName() << " " << station.getDistrict() << " "
                                     << station.getMunicipality() << endl;
                            }
                        }
                        cout << "|---------------------------------------------|" << endl;
                        if (noGood) {
                            cout << "Invalid Municipality: " << line << endl;
                        }
                        break;
                    }
                    case '4':{
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        bool noGood = true;
                        string line;
                        cout << "Enter the District: ";
                        getline(cin, line);
                        cout << "|---------------------------------------------|" << endl;
                        for(auto & station : ges.getStations()) {
                            if(station.getDistrict() == line) {
                                noGood = false;
                                cout << station.getName() << " " << station.getDistrict() << endl;
                            }
                        }
                        cout << "|---------------------------------------------|" << endl;
                        if(noGood) {
                            cout << "Invalid district: " << line << endl;
                        }
                    }
                        break;
                    case '5':{
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "|---------------------------------------------|" << endl;
                        for(auto station : ges.getNetwork().vertexs) {
                            cout << "Station: " << station.getName() << " " << station.getDistrict() << " " << station.getLine() << endl;
                            for(auto edge : station.getAdj()) {
                                cout << "   Connection: " << ges.getNetwork().vertexs[edge.getPointA()].getName() << " -> " << ges.getNetwork().vertexs[edge.getPointB()].getName() << endl;
                            }
                        }
                        cout << "|---------------------------------------------|" << endl;
                        break;
                    }
                    case '6':{
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        bool noGood = true;
                        string line;
                        cout << "Enter the line: ";
                        getline(cin, line);
                        line = line.append("\r");
                        cout << "|---------------------------------------------|" << endl;
                        for(auto station : ges.getNetwork().vertexs) {
                            if(station.getLine() != line) continue;
                            noGood = false;
                            cout << "Station: " << station.getName() << " " << station.getDistrict() << " " << station.getLine() << endl;
                            for(auto edge : station.getAdj()) {
                                cout << "   Connection: " << ges.getNetwork().vertexs[edge.getPointA()].getName() << " -> " << ges.getNetwork().vertexs[edge.getPointB()].getName() << endl;
                            }
                        }
                        cout << "|---------------------------------------------|" << endl;
                        if(noGood) cout << "Invalid line: " << line << endl;
                        break;
                    }
                    case '7': {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        string source, dest;
                        cout << "Enter the source station: ";
                        getline(cin, source);
                        cout << "Enter the destination station: ";
                        getline(cin, dest);
                        if (ges.pathIsOk(source, dest)) {
                            cout << "A path exists between " << source << " and " << dest << endl;
                        } else {
                            cout << "No path exists between " << source << " and " << dest << endl;
                        }
                        break;
                    }
                    case '0':
                        break;
                    default:
                        cout << "Invalid choice, please try again" << endl;
                        break;
                }
                break;
            }
            case '9':{
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Resetting network to its original values..." << endl;
                ges.getNetwork().resetResiduals();
                ges.setOK();
                break;
            }
            case '0': {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Exiting..." << endl;
                break;
            }
            default: {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid choice, please try again" << endl;
                break;
            }
        }
    } while(choice != '0');
    return 0;
};