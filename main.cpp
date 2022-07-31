#include "graph.h"
using namespace std;

//Function to load the states and unique codes onto a map
void loadStates(string filen, map<string, string> &stateAbbvs, map<string, int> &stateCodes){
    fstream inFile;

    inFile.open(filen, ios::in);

    string line;

    if(inFile.is_open()){
        
        while(getline(inFile, line)){
            stringstream ss(line);

            string stateName;
            string stateAbbv;
            int stateCode;

            // Getting the state code from the CSV file
            string temp = "";
            getline(ss, temp, ',');
            stateCode = atoi(temp.c_str());

            // Getting the state name from the CSV file
            getline(ss, stateName, ',');

            // Getting the state Abbreviation from the CSV file
            getline(ss, stateAbbv, ',');

            // Assigning the unique ids to the states
            
            if(!stateAbbvs.count(stateAbbv)){
                stateAbbvs[stateAbbv] = stateName;
            }
            if(!stateCodes.count(stateAbbv)){
                stateCodes[stateAbbv] = stateCode;
            }
            line = "";
        }

    }
}

int main(){
    map<string, string> stateAbv;
    map<string, int> stateCod;
    graph graph;
    //graph.loadGraph("Flights.csv");

    loadStates("StateCodes.csv", stateAbv, stateCod);

}
