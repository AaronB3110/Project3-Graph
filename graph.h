#define STATENUM 50
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <queue>
#include <stack>
#include <algorithm>
#pragma once

using namespace std;
// Structure to hold the information of states
/*struct vertex
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
    
};*/

// Class to create the data structure of a graph
class graph
{
private:
    //map<string, int> mapCodes;
    //map<string, string> mapAbbvs;
    pair<int, int> stateGraph[STATENUM][STATENUM];
public:
    graph();
    void loadGraph(string filen, map<string, int> mapCode);
    void DFS(int from, vector<pair<int,int>>& positions);
    vector<pair<pair<int, int>, int>> getAdj(int i);
    void BFS(int from, vector<pair<int, int>> &input);
};