#define STATENUM 50
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;
// Structure to hold the information of states
struct vertex
{
    string name;
    int passengers;
    int milesflown;

    vertex(string name, int passengers, int milesFlown){
        this->name = name;
        this->passengers = passengers;
        this->milesflown = milesFlown;
    };
    
};

// Class to create the data structure of a graph
class graph
{
    map<string, int> mapCodes;
    map<string, string> mapAbbvs;
    vertex stateGraph[STATENUM][STATENUM];

public:
    graph(/* args */);
    void loadGraph(string filen);
    void addEdge(string from, string to, vertex vertex);
    void updatePassenger(string to, int passengers);
    void updateMiles(string to, int miles);
    
    ~graph();
    
};

graph::graph(/* args */)
{
};

graph::~graph()
{
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
    int passengers;
    int milesFlown;

    // Only load the graph if the file is open
    if(inFile.is_open()){
        string line = "";
        cout << "Hello World";
        while(getline(inFile, line)){
            
            stringstream ss(line);

            getline(ss, from, ',');

            getline(ss, to, ',');

            string tempPass;
            getline(ss, tempPass, ',');
            passengers = atoi(tempPass.c_str());

            string tempMiles;
            getline(ss, tempMiles, ',');
            milesFlown = atoi(tempMiles.c_str());


            line = "";
        }
    }

}

void graph::addEdge(string from, string to, vertex vertex){
    stateGraph[mapCodes[from]][mapCodes[to]] = vertex;
}


