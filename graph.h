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
// Class to create the data structure of a graph
class graph
{
public:
    pair<int, int> stateGraph[STATENUM][STATENUM];
    graph();
    void loadGraph(string filen, map<string, int>& mapCode);
    void DFS(int from, vector<pair<int,int>>& positions);
    void getAdj(int i, vector<pair<pair<int, int>,int>>& adj, set<int>& visited);
    void BFS(int from, vector<pair<int, int>> &input);
};