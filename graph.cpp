#include "graph.h"
graph::graph(){
    pair<int, int> stateGraph[STATENUM][STATENUM] = {make_pair(0,0)}; // Graph implement First : Passengers, Second: Miles
}

void graph::loadGraph(string filen, map<string, int>& mapCodes ){
    // Object to read CSV file with flight info
    fstream inFile;

    // Open csv file
    inFile.open(filen, ios::in);

    // Remove the header from the csv file.
    string header;
    getline(inFile, header);

    // Variables to hold the csv data
    string from, to;
    int passengers = 0;
    int milesFlown = 0;

    // Only load the graph if the file is open
    if(inFile.is_open()){
        string line = "";
        //cout << "Hello World" << endl; // Test if file open
        while(getline(inFile, line)){
            // Parse the line
            stringstream ss(line);

            // Collect the abreviation from the state FROM
            getline(ss, from, ',');

            // Collect the abreviation from the state TO
            getline(ss, to, ',');

            // Collect the amount of passengers in the flight
            string tempPass;
            getline(ss, tempPass, ',');
            passengers = stoi(tempPass);

            // Collect the miles flown in the flight;
            string tempMiles;
            getline(ss, tempMiles, ',');
            milesFlown = stoi(tempMiles);

            //insert data into the adjacency matrix
            // Check if the graph has any information in it

            int fromMap = mapCodes[from];
            int toMap = mapCodes[to];
            if(stateGraph[fromMap][toMap] == make_pair(0,0)){
                // change the data in the position of the vertex if there is no data
                stateGraph[fromMap][toMap].first = passengers;
                stateGraph[fromMap][toMap].second = milesFlown;
            } else {
                // update the data in the position of the vertex if there is data
                stateGraph[fromMap][toMap].first += passengers;
                if(milesFlown < stateGraph[fromMap][toMap].second){
                    stateGraph[fromMap][toMap].second = milesFlown;
                }
            }

            // Reset every value
            tempMiles = "";
            tempPass = "";
            passengers = 0;
            milesFlown = 0;
            line = "";
        }
    }
}

void graph::DFS(int from, vector<pair<int,int>>& positions)
{
    set<pair<int,int>> visited;
    set<int> visitedState;
    stack<pair<int,int>> stackDFS;
    vector < pair<pair<int, int>, int >> distance;

    // Insert source nodes into stack and into visited set
    visited.insert(make_pair(from, from));
    stackDFS.push(make_pair(from, from));

    // While there are neighbors still checking for them and insert the position into the vector
    while (!stackDFS.empty())
    {
        int currState = stackDFS.top().first;
        int nextState = stackDFS.top().second;

        if(visitedState.count(nextState) != 0 && visitedState.count(currState) == 0)
        {
            nextState = currState;
        }
        stackDFS.pop();

        vector<pair<pair<int, int>, int>> neighbors;
        graph::getAdj(nextState, neighbors, visitedState);

        for (int i = 0; i < neighbors.size(); i++) { //iterate thru adj

            pair<int, int> adjState = neighbors[i].first;

            if(adjState.second < STATENUM && adjState.second >= 0 && adjState.first < STATENUM && adjState.first >= 0)
            {
                if (visited.count(adjState) == 0) {
                    visited.insert(adjState);
                    visited.insert(make_pair(adjState.second, adjState.first));

                    positions.push_back(adjState);

                    stackDFS.push(adjState);
                }
            }
        }
    }
}

void graph::getAdj(int i, vector<pair<pair<int, int>,int>>& adj, set<int>& visited)
{
    visited.insert(i);

    vector<pair< int, pair<int, int>>> neighbors;

    for (int l = 0; l < STATENUM; l++)
    {
        if (stateGraph[i][l].first != 0)
        {
            neighbors.push_back(make_pair(stateGraph[i][l].second ,make_pair(i, l)));
        }

    }

    sort(neighbors.begin(), neighbors.end());


    if(neighbors.size() == 0)
    {
        return;
    }
    for (int k = 0; k < 11; k++) {
        if(k < neighbors.size() && k != neighbors[k].second.second)
        {
            adj.push_back(make_pair(make_pair(neighbors[k].second.first, neighbors[k].second.second), neighbors[k].first));
        }
    }
}

void graph::BFS(int from, vector<pair<int, int>>& input)
{
    int source = from;

    set<pair<int, int>> visited;
    set<int> visitedState;
    queue<pair<int,int>> queState;
    vector < pair<pair<int, int>, int >> distance;


    visited.insert(make_pair(source, source));
    queState.push(make_pair(source,source));

    while (!queState.empty())
    {
        int currState = queState.front().first;
        int nextState = queState.front().second;

        if(visitedState.count(nextState) != 0 && visitedState.count(currState) == 0)
        {
            nextState = currState;
        }
        queState.pop();

        vector<pair<pair<int, int>, int>> neighbors;
        graph::getAdj(nextState, neighbors, visitedState);

        for (int i = 0; i < neighbors.size(); i++) { //iterate thru adj

            pair<int, int> adjState = neighbors[i].first;

            if(adjState.second < STATENUM && adjState.second >= 0 && adjState.first < STATENUM && adjState.first >= 0)
            {
                if (visited.count(adjState) == 0) {
                    visited.insert(adjState);
                    visited.insert(make_pair(adjState.second, adjState.first));

                    input.push_back(adjState);

                    queState.push(adjState);
                }
            }
        }
    }
}

