#include "graph.h"
graph::graph(){
    map<string, int> mapCodes;
    map<string, string> mapAbbvs;
    vertex stateGraph[STATENUM][STATENUM];
}
void graph::loadGraph(string filen){
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
            
            stringstream ss(line);

            getline(ss, from, ',');

            getline(ss, to, ',');

            string tempPass;
            getline(ss, tempPass, ',');
            passengers = stoi(tempPass);
            cout << passengers << " ";

            string tempMiles;
            getline(ss, tempMiles, ',');
            milesFlown = stoi(tempMiles);
            cout << milesFlown << endl;

            tempMiles = "";
            tempPass = "";
            line = "";
        }
    }
}

void graph::addEdge(string from, string to, vertex vertex){
    stateGraph[mapCodes[from]][mapCodes[to]] = vertex;
}
