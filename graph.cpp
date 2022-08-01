#include "graph.h"
graph::graph(){
    pair<int, int> stateGraph[STATENUM][STATENUM] = {make_pair(0,0)}; // Graph implement First : Passengers, Second: Miles
}

void graph::loadGraph(string filen, map<string, int> mapCodes){
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
        cout << "Hello World" << endl;
        while(getline(inFile, line)) {
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
            if (stateGraph[mapCodes[from]][mapCodes[to]] == make_pair(0, 0)) {
                // change the data in the position of the vertex if there is no data
                stateGraph[mapCodes[from]][mapCodes[to]].first = passengers;
                stateGraph[mapCodes[from]][mapCodes[to]].second = milesFlown;
            } else {
                // update the data in the position of the vertex if there is data
                stateGraph[mapCodes[from]][mapCodes[to]].first += passengers;
                if (milesFlown < stateGraph[mapCodes[from]][mapCodes[to]].second) {
                    stateGraph[mapCodes[from]][mapCodes[to]].second = milesFlown;
                }
            }

            /*// Reset every value
            tempMiles = "";
            tempPass = "";
            passengers = 0;
            milesFlown = 0;
            line = "";*/


        }
    }
}

void graph::DFS(int from, vector<pair<int,int>>& positions){
    set<int> visitedState;
    stack<int> stackDFS;

    int smallestMiles = 1000;
    int positionY = 0;

    // Insert source nodes into stack and into visited set
    visitedState.insert(from);
    stackDFS.push(from);

    // While there are neighbors still checking for them and insert the position into the vector
    while(!stackDFS.empty()){
        smallestMiles = 1000;
        from = stackDFS.top();
        stackDFS.pop();

        for(int j = 0; j < STATENUM; j++){
            if(stateGraph[from][j].second != 0){
                
            }
        }
    }
}

vector<pair<pair<int, int>, int>> graph::getAdj(int i) { //i current node, from is user input (using from so that we don't get back links)

    vector<pair< int, pair<int, int>>> neighbors;// distance, indexes


    for (int w = 0; w <= STATENUM; w++) {

        for (int y = 0; y < STATENUM; y++) {
            if (w == i) {
                //if (y!= from) {
                pair<int, int> vertexData = stateGraph[w][y];
                neighbors.push_back(make_pair(stateGraph[w][y].second, make_pair(w, y)));
                //}

            }

        }
    }

    sort(neighbors.begin(), neighbors.end());

    vector < pair<pair<int, int>, int>> top4Neighbors;
    int numToTake = 0;

    for (int k = 0; k < neighbors.size(); k++) {

        // if (neighbors[k].second.second !=0) {
        top4Neighbors.push_back(make_pair(neighbors[k].second, neighbors[k].first));
        numToTake++;
        //}

        if (numToTake == 4) {
            break;
        }
    }


    return top4Neighbors;


}

void graph::BFS(int from, vector<pair<int, int>>& input) {

    int source = from;

    set<pair<int, int>> visited;
    queue<int> queState;
    vector < pair<pair<int, int>, int >> distance;

    vector<pair<int, int>> posOfAdjacents = input;

    visited.insert(make_pair(source, source));
    queState.push(source);

    while (queState.empty() != true) {
        int currState = queState.front();
        queState.pop();

        vector<pair<pair<int, int>, int>> neighbors = graph::getAdj(currState);


        for (int i = 0; i < neighbors.size(); i++) { //iterate thru adj

            pair<int, int> adjState = neighbors[i].first; //indexes of adjacent vertexes

            if (visited.count(adjState) == 0) { //if vertices has not been visited
                visited.insert(adjState); //insert vertex into visited set

                input.push_back(adjState); //push vertex into return vector

                queState.push(neighbors[i].first.second); //push the "to" value into the queue

            }
        }

    }


}