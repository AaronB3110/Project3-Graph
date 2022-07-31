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

