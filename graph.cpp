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
            if(stateGraph[mapCodes[from]][mapCodes[to]] == make_pair(0,0)){
                // change the data in the position of the vertex if there is no data
                stateGraph[mapCodes[from]][mapCodes[to]].first = passengers;
                stateGraph[mapCodes[from]][mapCodes[to]].second = milesFlown;
            } else {
                // update the data in the position of the vertex if there is data
                stateGraph[mapCodes[from]][mapCodes[to]].first += passengers;
                if(milesFlown < stateGraph[mapCodes[from]][mapCodes[to]].second){
                    stateGraph[mapCodes[from]][mapCodes[to]].second = milesFlown;
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

void graph::DFS(int from, vector<pair<int,int>>& positions){
    set<pair<int,int>> visitedPost;
    stack<pair<int,int>> stackDFS;

    int smallestMiles = 10000;
    int positionY;

    // Insert source nodes into stack and into visited set
    visitedPost.insert(make_pair(from, from));
    stackDFS.push(make_pair(from, from));

    // While there are neighbors still checking for them and insert the position into the vector
    while(!stackDFS.empty()){
        from = stackDFS.top().first;
        stackDFS.pop();

        for(int i = 0; i < 4; i++){
            for(int j = 0; j < STATENUM; j++){
                visitedPost.insert(make_pair(from, j));
                if(!visitedPost.count(make_pair(from, j))){

                }
                if(stateGraph[from][j].second < smallestMiles){
                    smallestMiles = stateGraph[from][j].second;
                    positionY = j;
                }
            }
            positions.push_back(make_pair(from, positionY));
        }
    }
}

vector<pair<pair<int, int>, int>> graph::getAdj(int i) {

    vector<pair< int, pair<int, int>>> neighbors;

    for (int l = 0; l < STATENUM; l++) {
        neighbors.push_back(make_pair(stateGraph[i][l].first ,make_pair(i, l)));

    }

    sort(neighbors.begin(), neighbors.end());

    vector < pair<pair<int, int>, int>> top4Neighbors;

    for (int k = 0; k < 4; k++) {
        top4Neighbors.push_back(make_pair(neighbors[k].second, neighbors[k].first));
    }


    return top4Neighbors;


}

void graph::BFS(int from, vector<pair<int, int>> &input) {

    int source = from;

    set<pair<int, int>> visited;
    queue<pair<int, int>> queState;
    vector < pair<pair<int, int>, int >> distance;

    vector<pair<int, int>> posOfAdjacents = input;

    visited.insert(make_pair(source, source));
    queState.push(make_pair(source, source));

    while (queState.empty() != true) {
        pair<int,int> currState = queState.front();
        queState.pop();

        vector<pair<pair<int, int>, int>> neighbors = graph::getAdj(currState.first);


        for (int i = 0; i < neighbors.size(); i++) { //iterate thru adj

            pair<int, int> adjState = neighbors[i].first;

            if (visited.count(adjState) == 0) {
                visited.insert(adjState);

                input.push_back(adjState);

                queState.push(adjState);

            }
        }

    }


}

