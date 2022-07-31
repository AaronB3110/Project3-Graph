#define STATENUM 50
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#pragma once

using namespace std;
// Structure to hold the information of states
struct vertex
{
    string name;
    int passengers;
    int milesflown;

    vertex(){
        this->name = "default";
        this->passengers = 0;
        this->milesflown = 0;
    }
    
    vertex(string name, int passengers, int milesFlown){
        this->name = name;
        this->passengers = passengers;
        this->milesflown = milesFlown;
    };
    
};

// Class to create the data structure of a graph
class graph
{
private:
    map<string, int> mapCodes;
    map<string, string> mapAbbvs;
    vertex stateGraph[STATENUM][STATENUM];
public:
    graph();
    void loadGraph(string filen);
    void addEdge(string from, string to, vertex vertex);
    void updatePassenger(string to, int passengers);
    void updateMiles(string to, int miles);
};