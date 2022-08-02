/*-----References-----*/
//SFML : https://www.sfml-dev.org/tutorials/2.5/
//Map Image : https://freesvg.org/outline-map-of-american-states
//Aircraft Image : https://commons.wikimedia.org/wiki/File:Airplane_silhouette_S,_flipped.png
//font : https://www.dafont.com/
//c++ : https://cplusplus.com
//Uses SMFL 2.5
#include <SFML/Graphics.hpp>
#include "graph.h"
#include <iostream>
#include <vector>
#include <map>
#include <chrono>
#include <set>
using namespace std;

void setScreen(sf::RenderWindow& _window);
void stateList(graph& graph, sf::RenderWindow& _window, sf::Font& font_1, map<int, pair<string,string>>& mapping, vector<pair<int,int>>& traversal, int index);

void circles(map<int, pair<string,string>>& mapping, vector<sf::CircleShape>& state, map<int, pair<float,float>>& cords, map<string, int>& sMap);
void makeVertex(sf::CircleShape& vertex, float x, float y, int i, string abr, string full, map<int, pair<string,string>>& mapping, vector<sf::CircleShape>& state, map<int, pair<float,float>>& cords, map<string, int>& sMap);
void makeEdge(sf::RenderWindow& _window, map<int, pair<float,float>>& cords, vector<pair<sf::Vertex, sf::Vertex>>& edge, set<pair<int,int>>& visited, vector<pair<int,int>>& traversal, vector<sf::CircleShape>& states, int index, int& speed, int& speedRed, int speedFactor);

void text(sf::RenderWindow& _window, sf::Font& font_1, string stateName, bool algBFS, bool algDFS, int miFlown, int numPassenger, string cState, int time, int speedFactor);
void createText(sf::Text& obj, sf::Font& font, string text, int size, float x, float y);

int main()
{
    map<string, string> stateAbv;
    map<string, int> stateCod;
    graph graph;

    map<int, pair<string,string>> stateMapping; // number - abbreviation - full name
    map<string, int> stringMap; // maps full length state name to int, same map as int to string
    map<int, pair<float,float>> stateCords; // X-Y coordinates of each state

    vector<sf::CircleShape> states; // stores the physical vertices
    vector<pair<sf::Vertex, sf::Vertex>> edge; // stores edge vertices
    set<pair<int,int>> existingEdge; // store edges that have already been visualized
    vector<pair<int,int>> traversal; // stores the graph traversal edges

    sf::Font font_1;
    font_1.loadFromFile("data/Altone.ttf");

    sf::RectangleShape bfsButton(sf::Vector2f(100.f, 40.f));
    bfsButton.setFillColor(sf::Color(130,130,130));
    bfsButton.setPosition(135.0, 70.0);

    sf::RectangleShape dfsButton(sf::Vector2f(100.f, 40.f));
    dfsButton.setFillColor(sf::Color(130,130,130));
    dfsButton.setPosition(265.0, 70.0);

    sf::RectangleShape resetButton(sf::Vector2f(110.f, 40.f));
    resetButton.setFillColor(sf::Color(130,130,130));
    resetButton.setPosition(195.0, 1030.0);

    sf::RectangleShape restartVis(sf::Vector2f(120.f, 40.f));
    restartVis.setFillColor(sf::Color(130,130,130));
    restartVis.setPosition(330.0, 1030.0);

    sf::RectangleShape speedUP(sf::Vector2f(50.f, 20.f));
    speedUP.setFillColor(sf::Color(130,130,130));
    speedUP.setPosition(113.0, 1050.0);

    sf::RectangleShape speedDOWN(sf::Vector2f(50.f, 20.f));
    speedDOWN.setFillColor(sf::Color(130,130,130));
    speedDOWN.setPosition(52.0, 1050.0);

    bool isDFSSelected = false; // if dfs is selected
    bool isBFSSelected = false; // if bfs is selected
    bool algSelected = false; // if an algorithm is selected

    circles(stateMapping, states, stateCords, stringMap); // create vertices for each state
    vector<bool> click(states.size(),false); // keeps track of which state is clicked
    bool stateSelected = false;

    graph.loadGraph("data/Flights_Full.csv", stringMap);

    int index = INT_MAX; // index of user selected state
    string name = "SELECT STATE";
    int numPassengers = 0;
    int mileFlown = 0;
    string closest = "-----";
    int renderSpeed = 0.0;
    int speedReducer = 0;
    int timeElapsed = 0;
    int speedFactor = 5;

    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Flight");

    while(window.isOpen())
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == event.MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
            {
                if(resetButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                {
                    stateSelected = false;
                    algSelected = false;
                    index = INT_MAX;
                    name = "SELECT STATE";
                    closest = "-----";
                    numPassengers = 0;
                    mileFlown = 0;
                    renderSpeed = 0;
                    speedReducer = 0;
                    timeElapsed = 0.0;
                    speedFactor = 5;

                    for(int i = 0; i < states.size(); i++) // reset all vertices
                    {
                        click[i] = false;
                        states[i].setFillColor(sf::Color::Green);
                    }

                    isBFSSelected = false;
                    bfsButton.setFillColor(sf::Color(130,130,130));

                    isDFSSelected = false;
                    dfsButton.setFillColor(sf::Color(130,130,130));

                    existingEdge.clear();
                    edge.clear();
                    traversal.clear();
                }
                else if(restartVis.getGlobalBounds().contains(mousePos.x, mousePos.y)) // restart traversal visualization
                {
                    if(algSelected)
                    {
                        for(int i = 0; i < states.size(); i++) // reset all vertices
                        {
                            states[i].setFillColor(sf::Color::Green);
                        }

                        renderSpeed = 0;
                        speedReducer = 0;
                        existingEdge.clear();
                        edge.clear();
                    }
                }
                else if(speedUP.getGlobalBounds().contains(mousePos.x, mousePos.y)) // increase speed of render
                {
                    if(speedFactor != 1 && algSelected)
                    {
                        speedUP.setFillColor(sf::Color(60,60,60));
                        speedFactor --;
                    }

                }
                else if(speedDOWN.getGlobalBounds().contains(mousePos.x, mousePos.y)) // decrease speed of render
                {
                    if(speedFactor != 20 && algSelected)
                    {
                        speedDOWN.setFillColor(sf::Color(60,60,60));
                        speedFactor ++;
                    }
                }
                else
                {
                    if(!algSelected)
                    {
                        if(bfsButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) // bfs algorithm button
                        {
                            if(stateSelected && !isDFSSelected)
                            {
                                isBFSSelected = !isBFSSelected;
                                algSelected = !algSelected;
                                bfsButton.setFillColor(sf::Color(60,60,60));

                                auto start = std::chrono::high_resolution_clock::now();

                                graph.BFS(index, traversal);

                                auto stop = std::chrono::high_resolution_clock::now();
                                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
                                timeElapsed = duration.count();

                                closest = stateMapping[traversal[0].second].second;
                                numPassengers = graph.stateGraph[index][traversal[0].second].first;
                                mileFlown = graph.stateGraph[index][traversal[0].second].second;
                            }
                        }
                        else if(dfsButton.getGlobalBounds().contains(mousePos.x, mousePos.y))// dfs algorithm button
                        {
                            if(stateSelected && !isBFSSelected)
                            {
                                isDFSSelected = !isDFSSelected;
                                algSelected = !algSelected;
                                dfsButton.setFillColor(sf::Color(60,60,60));

                                auto start = std::chrono::high_resolution_clock::now();

                                graph.DFS(index, traversal);

                                auto stop = std::chrono::high_resolution_clock::now();
                                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
                                timeElapsed = duration.count();

                                closest = stateMapping[traversal[0].second].second;
                                numPassengers = graph.stateGraph[index][traversal[0].second].first;
                                mileFlown = graph.stateGraph[index][traversal[0].second].second;
                            }
                        }
                        for(int i = 0; i < states.size(); i++) // iterate through all the states to determine which one was clicked
                        {
                            if (mousePos.x < (states[i].getGlobalBounds().left + 28.0) && mousePos.x > (states[i].getGlobalBounds().left) && mousePos.y > (states[i].getGlobalBounds().top) && mousePos.y < (states[i].getGlobalBounds().top + 28.0)) // florida
                            {
                                if(!stateSelected)
                                {
                                    click[i] = !click[i];
                                    stateSelected = !stateSelected;
                                    states[i].setFillColor(sf::Color::Blue);
                                    index = i;
                                    name = stateMapping[i].second;
                                }
                                else if(stateSelected && click[i] && !algSelected) // if state deselected
                                {
                                    click[i] = !click[i];
                                    stateSelected = !stateSelected;
                                    states[i].setFillColor(sf::Color::Green);
                                    index = INT_MAX;
                                    name = "SELECT STATE";
                                    mileFlown = 0;
                                }
                                break;
                            }
                        }
                    }
                }
            }
        }

        window.clear(sf::Color::White);

        setScreen(window);

        makeEdge(window, stateCords, edge, existingEdge, traversal, states, index, renderSpeed, speedReducer, speedFactor);
        for(auto iter:edge)
        {
            sf::Vertex line[] = {
                    iter.first,
                    iter.second};
            line->color = sf::Color::Red;
            window.draw(line, 2, sf::Lines);
        }

        if(index != INT_MAX)
        {
            states[index].setFillColor(sf::Color::Blue);
        }
        for(auto iter:states)
        {
            window.draw(iter);
        }

        window.draw(dfsButton);
        window.draw(bfsButton);
        window.draw(resetButton);
        window.draw(restartVis);
        window.draw(speedUP);
        window.draw(speedDOWN);

        text(window, font_1, name, isBFSSelected, isDFSSelected, mileFlown, numPassengers, closest, timeElapsed, speedFactor);

        if(algSelected)
        {
            stateList(graph, window, font_1, stateMapping, traversal, index);
        }

        window.display();

        speedUP.setFillColor(sf::Color(130,130,130));
        speedDOWN.setFillColor(sf::Color(130,130,130));
    }
    return 0;
}
void setScreen(sf::RenderWindow& _window)
{
    sf::Texture usMap;
    usMap.loadFromFile("data/map.png");
    usMap.setSmooth(true);

    sf::Texture plane;
    plane.loadFromFile("data/airplane.png");
    plane.setSmooth(true);

    sf::RectangleShape mapSprite(sf::Vector2f(1404.f, 993.f));
    mapSprite.setPosition(520.0 ,35.0);
    mapSprite.setTexture(&usMap);

    sf::RectangleShape menu(sf::Vector2f(500.f, 1080.f));
    menu.setPosition(0.0 ,0.0);
    menu.setFillColor(sf::Color(190,190,190));

    sf::RectangleShape titleBackground(sf::Vector2f(250.f, 30.f));
    titleBackground.setPosition(125.0 ,25.0);
    titleBackground.setFillColor(sf::Color(235,235,235));

    sf::RectangleShape stateListBackground(sf::Vector2f(400.f, 780.f));
    stateListBackground.setFillColor(sf::Color(235,235,235));
    stateListBackground.setPosition(50.0, 240.0);

    sf::RectangleShape airplane(sf::Vector2f(100.f, 47.f));
    airplane.setPosition(10.0 ,25.0);
    airplane.setTexture(&plane);

    sf::RectangleShape headingLine(sf::Vector2f(400.f, 1.f));
    headingLine.setFillColor(sf::Color::Black);
    headingLine.setPosition(50.0, 255.0);

    sf::RectangleShape speed(sf::Vector2f(120.f, 16.f));
    speed.setFillColor(sf::Color(130,130,130));
    speed.setPosition(50.0, 1030.0);

    _window.draw(menu);
    _window.draw(mapSprite);
    _window.draw(titleBackground);
    _window.draw(stateListBackground);
    _window.draw(airplane);
    _window.draw(headingLine);
    _window.draw(speed);
}
void stateList(graph& graph, sf::RenderWindow& _window, sf::Font& font_1, map<int, pair<string,string>>& mapping, vector<pair<int,int>>& traversal, int index)
{
    set<int> visited;
    visited.insert(index);
    vector<int> mileTotal(50, 0); // stores the distance by combining the distance of the origin vertex to the destination vertex
    vector<int> passTotal(50,0); // stores total number of passengers

    int order = 1;
    float y = 245.0;
    for(int i = 0; i < traversal.size(); i++)
    {
        if(visited.count(traversal[i].second) == 0)
        {
            visited.insert(traversal[i].second);
            y += 15.0;
            mileTotal[traversal[i].second] = mileTotal[traversal[i].first] + graph.stateGraph[traversal[i].first][traversal[i].second].second;
            passTotal[traversal[i].second] = passTotal[traversal[i].first] + graph.stateGraph[traversal[i].first][traversal[i].second].first;

            string r = to_string(order);
            sf::Text rank;
            createText(rank, font_1, r, 14, 70.5, y);
            order ++;

            sf::Text state;
            createText(state, font_1, mapping[traversal[i].second].second, 14, 150.0, y);

            string p = to_string(passTotal[traversal[i].second]);//graph.stateGraph[traversal[i].first][traversal[i].second].first
            sf::Text passenger;
            createText(passenger, font_1, p, 14, 290.0, y);

            string m = to_string(mileTotal[traversal[i].second]);
            sf::Text mile;
            createText(mile, font_1, m, 14, 380.0, y);

            _window.draw(rank);
            _window.draw(state);
            _window.draw(passenger);
            _window.draw(mile);
        }
    }
}

void circles(map<int, pair<string,string>>& mapping, vector<sf::CircleShape>& state, map<int, pair<float,float>>& cords, map<string, int>& sMap)
{
    sf::CircleShape AL(15.f); // Alabama
    makeVertex(AL,1455.0,630.0, 0, "AL", "Alabama", mapping, state, cords, sMap);

    sf::CircleShape AK(15.f); // Alaska
    makeVertex(AK, 890.0, 830.0, 1, "AK", "Alaska", mapping, state, cords, sMap);

    sf::CircleShape AZ(15.f); // Arizona
    makeVertex(AZ, 830.0, 600.0, 2, "AZ", "Arizona", mapping, state, cords, sMap);

    sf::CircleShape AR(15.f); // Arkansas
    makeVertex(AR,1305.0, 585.0, 3, "AR", "Arkansas", mapping, state, cords, sMap);

    sf::CircleShape CA(15.f); // California
    makeVertex(CA, 650.0, 450.0, 4, "CA", "California", mapping, state, cords, sMap);

    sf::CircleShape CO(15.f); // Colorado
    makeVertex(CO, 980.0, 445.0, 5, "CO", "Colorado", mapping, state, cords, sMap);

    sf::CircleShape CT(15.f); // Connecticut
    makeVertex(CT, 1725.0, 305.0, 6, "CT", "Connecticut", mapping, state, cords, sMap);

    sf::CircleShape DE(15.f); // Delaware
    makeVertex(DE, 1690.0, 400.0, 7, "DE", "Delaware", mapping, state, cords, sMap);

    sf::CircleShape FL(15.f); // Florida
    makeVertex(FL, 1595.0, 760.0, 8, "FL", "Florida", mapping, state, cords, sMap);

    sf::CircleShape GA(15.f); // Georgia
    makeVertex(GA, 1535.0, 630.0, 9, "GA", "Georgia", mapping, state, cords, sMap);

    sf::CircleShape HI(15.f); // Hawaii
    makeVertex(HI, 680.0, 815.0, 10, "HI", "Hawaii", mapping, state, cords, sMap);

    sf::CircleShape ID(15.f); // Idaho
    makeVertex(ID, 810.0, 250.0, 11, "ID", "Idaho", mapping, state, cords, sMap);

    sf::CircleShape IL(15.f); // Illinois
    makeVertex(IL, 1365.0, 395.0, 12, "IL", "Illinois", mapping, state, cords, sMap);

    sf::CircleShape IN(15.f); // Indiana
    makeVertex(IN, 1440.0, 395.0, 13, "IN", "Indiana", mapping, state, cords, sMap);

    sf::CircleShape IA(15.f); // Iowa
    makeVertex(IA, 1285.0, 360.0, 14, "IA", "Iowa", mapping, state, cords, sMap);

    sf::CircleShape KS(15.f); // Kansas
    makeVertex(KS, 1150.0, 465.0, 15, "KS", "Kansas", mapping, state, cords, sMap);

    sf::CircleShape KY(15.f); // Kentucky
    makeVertex(KY, 1490.0, 475.0, 16, "KY", "Kentucky", mapping, state, cords, sMap);

    sf::CircleShape LA(15.f); // Louisiana
    makeVertex(LA, 1315.0, 700.0, 17, "LA", "Louisiana", mapping, state, cords, sMap);

    sf::CircleShape ME(15.f); // Maine
    makeVertex(ME, 1776.0, 150.0, 18, "ME", "Maine", mapping, state, cords, sMap);

    sf::CircleShape MD(15.f); // Maryland
    makeVertex(MD, 1650.0, 400.0, 19, "MD", "Maryland", mapping, state, cords, sMap);

    sf::CircleShape MA(15.f); // Massachusetts
    makeVertex(MA, 1745.0, 275.0, 20, "MA", "Massachusetts", mapping, state, cords, sMap);

    sf::CircleShape MI(15.f); // Michigan
    makeVertex(MI, 1455.0, 290.0, 21, "MI", "Michigan", mapping, state, cords, sMap);

    sf::CircleShape MN(15.f); // Minnesota
    makeVertex(MN, 1240.0, 200.0, 22, "MN", "Minnesota", mapping, state, cords, sMap);

    sf::CircleShape MS(15.f); // Mississippi
    makeVertex(MS, 1380.0, 640.0, 23, "MS", "Mississippi", mapping, state, cords, sMap);

    sf::CircleShape MO(15.f); // Missouri
    makeVertex(MO, 1305.0, 470.0, 24, "MO", "Missouri", mapping, state, cords, sMap);

    sf::CircleShape MT(15.f); // Montana
    makeVertex(MT, 940.0, 160.0, 25, "MT", "Montana", mapping, state, cords, sMap);

    sf::CircleShape NE(15.f); // Nebraska
    makeVertex(NE, 1120.0, 380.0, 26, "NE", "Nebraska", mapping, state, cords, sMap);

    sf::CircleShape NV(15.f); // Nevada
    makeVertex(NV, 750.0, 400.0, 27, "NV", "Nevada", mapping, state, cords, sMap);

    sf::CircleShape NH(15.f); // New Hampshire
    makeVertex(NH, 1745.0, 240.0, 28, "NH", "New Hampshire", mapping, state, cords, sMap);

    sf::CircleShape NJ(15.f); // New Jersey
    makeVertex(NJ, 1700.0, 350.0, 29, "NJ", "New Jersey", mapping, state, cords, sMap);

    sf::CircleShape NM(15.f); // New Mexico
    makeVertex(NM, 980.0, 600.0, 30, "NM", "New Mexico", mapping, state, cords, sMap);

    sf::CircleShape NY(15.f); // New York
    makeVertex(NY, 1670.0, 280.0, 31, "NY", "New York", mapping, state, cords, sMap);

    sf::CircleShape NC(15.f); // North Carolina
    makeVertex(NC, 1620.0, 525.0, 32, "NC", "North Carolina", mapping, state, cords, sMap);

    sf::CircleShape ND(15.f); // North Dakota
    makeVertex(ND, 1120.0, 180.0, 33, "ND", "North Dakota", mapping, state, cords, sMap);

    sf::CircleShape OH(15.f); // Ohio
    makeVertex(OH, 1515.0, 395.0, 34, "OH", "Ohio", mapping, state, cords, sMap);

    sf::CircleShape OK(15.f); // Oklahoma
    makeVertex(OK, 1200.0, 580.0, 35, "OK", "Oklahoma", mapping, state, cords, sMap);

    sf::CircleShape OR(15.f); // Oregon
    makeVertex(OR, 680.0, 210.0, 36, "OR", "Oregon", mapping, state, cords, sMap);

    sf::CircleShape PA(15.f); // Pennsylvania
    makeVertex(PA, 1630.0, 350.0, 37, "PA", "Pennsylvania", mapping, state, cords, sMap);

    sf::CircleShape RI(15.f); // Rhode Island
    makeVertex(RI, 1760.0, 304.0, 38, "RI", "Rhode Island", mapping, state, cords, sMap);

    sf::CircleShape SC(15.f); // South Carolina
    makeVertex(SC, 1595.0, 585.0, 39, "SC", "South Carolina", mapping, state, cords, sMap);

    sf::CircleShape SD(15.f); // South Dakota
    makeVertex(SD, 1120.0, 280.0, 40, "SD", "South Dakota", mapping, state, cords, sMap);

    sf::CircleShape TN(15.f); // Tennessee
    makeVertex(TN, 1450.0, 545.0, 41, "TN", "Tennessee", mapping, state, cords, sMap);

    sf::CircleShape TX(15.f); // Texas
    makeVertex(TX, 1130.0, 700.0, 42, "TX", "Texas", mapping, state, cords, sMap);

    sf::CircleShape UT(15.f); // Utah
    makeVertex(UT, 870.0, 425.0, 43, "UT", "Utah", mapping, state, cords, sMap);

    sf::CircleShape VT(15.f); // Vermont
    makeVertex(VT, 1713.0, 210.0, 44, "VT", "Vermont", mapping, state, cords, sMap);

    sf::CircleShape VA(15.f); // Virginia
    makeVertex(VA, 1635.0, 460.0, 45, "VA", "Virginia", mapping, state, cords, sMap);

    sf::CircleShape WA(15.f); // Washington
    makeVertex(WA, 720.0, 120.0, 46, "WA", "Washington", mapping, state, cords, sMap);

    sf::CircleShape WV(15.f); // West Virginia
    makeVertex(WV, 1570.0, 435.0, 47, "WV", "West Virginia", mapping, state, cords, sMap);

    sf::CircleShape WI(15.f); // Wisconsin
    makeVertex(WI, 1355.0, 280.0, 48, "WI", "Wisconsin", mapping, state, cords, sMap);

    sf::CircleShape WY(15.f); // Wyoming
    makeVertex(WY, 975.0, 300.0, 49, "WY", "Wyoming", mapping, state, cords, sMap);
}
void makeVertex(sf::CircleShape& vertex, float x, float y, int i, string abr, string full, map<int, pair<string,string>>& mapping, vector<sf::CircleShape>& state, map<int, pair<float,float>>& cords, map<string, int>& sMap)
{
    vertex.setPosition(x, y);
    vertex.setFillColor(sf::Color::Green);
    mapping[i] = make_pair(abr, full);
    state.push_back(vertex);
    cords[i] = make_pair(x,y);
    sMap[abr] = i;
}
void makeEdge(sf::RenderWindow& _window, map<int, pair<float,float>>& cords, vector<pair<sf::Vertex, sf::Vertex>>& edge, set<pair<int,int>>& visited, vector<pair<int,int>>& traversal, vector<sf::CircleShape>& states, int index, int& speed, int& speedRed, int speedFactor)
{
    if(index != INT_MAX)
    {
        if(speedRed % speedFactor == 0)
        {
            if(speed <= traversal.size())
            {
                for(int j = 0; j < speed; j++) // arr[index].size()
                {
                    if(visited.count(make_pair(traversal[j].first, traversal[j].second)) == 0)
                    {
                        sf::Vertex line[] = {sf::Vertex(sf::Vector2f(cords[traversal[j].first].first+12.5, cords[traversal[j].first].second+12.5), sf::Color::Red, sf::Vector2f(1,1)),
                                             sf::Vertex(sf::Vector2f(cords[traversal[j].second].first+12.5, cords[traversal[j].second].second+12.5),sf::Color::Red, sf::Vector2f(1,1))};
                        edge.push_back(make_pair(line[0], line[1]));
                        states[traversal[j].first].setFillColor(sf::Color::Red);
                        states[traversal[j].second].setFillColor(sf::Color::Red);
                        visited.insert(make_pair(index, j));
                    }
                }
                speed ++;
            }
        }
        speedRed ++;
    }
}

void text(sf::RenderWindow& _window, sf::Font& font_1, string stateName, bool algBFS, bool algDFS, int miFlown, int numPassenger, string cState, int time, int speedFactor)
{
    sf::Text title;
    createText(title, font_1,stateName, 24,170.0, 25.0);

    sf::Text closest;
    string closeState = "Closest State: " + cState;
    createText(closest, font_1, closeState, 16, 139.0, 160.0);

    sf::Text passenger;
    if(numPassenger == 0)
    {
        createText(passenger, font_1, "Number of Passengers: -----", 16, 64.0, 185.0);
    }
    else
    {
        string numPass = "Number of Passengers: " + to_string(numPassenger);
        createText(passenger, font_1, numPass, 16, 64.0, 185.0);
    }

    sf::Text miles;
    if(miFlown == 0)
    {
        createText(miles, font_1, "Miles Flown: -----", 16, 155.0, 210.0);
    }
    else
    {
        string mileFlown = "Miles Flown: " + to_string(miFlown);
        createText(miles, font_1, mileFlown, 16, 155.0, 210.0);
    }

    sf::Text timeText;
    if(time == 0.0)
    {
        createText(timeText, font_1, "Function Execution Time: -----", 16, 51.0, 115.0);
    }
    else
    {
        string eTime = "Function Execution Time: " + to_string(time) + ".00 milliseconds";
        createText(timeText, font_1, eTime, 16, 51.0, 115.0);
    }

    sf::Text dfs;
    createText(dfs, font_1, "DFS", 24, 290.0, 75.0);
    if(algDFS)
    {
        dfs.setFillColor(sf::Color::White);
    }

    sf::Text bfs;
    createText(bfs, font_1, "BFS", 24, 165.0, 75.0);
    if(algBFS)
    {
        bfs.setFillColor(sf::Color::White);
    }

    sf::Text reset;
    createText(reset, font_1, "RESET", 24, 214.0, 1032);

    sf::Text restart;
    createText(restart, font_1, "RESTART", 14, 360.0, 1032);

    sf::Text vis;
    createText(vis, font_1, "VISUALIZATION", 14, 338.0, 1047);

    sf::Text visSpeed;
    createText(visSpeed, font_1, "RENDER SPEED", 14, 54.0, 1028);

    sf::Text rankList;
    createText(rankList, font_1, "Order", 16, 55.0, 236.0);

    sf::Text stateList;
    createText(stateList, font_1, "State", 16, 150.0, 236.0);

    sf::Text passList;
    createText(passList, font_1, "Passengers", 16, 250.0, 236.0);

    sf::Text mileList;
    createText(mileList, font_1, "Miles", 16, 380.0, 236.0);

    sf::Text up;
    if(speedFactor == 1)
    {
        createText(up, font_1, "MAX", 18, 118.0, 1048);
    }
    else
    {
        createText(up, font_1, "^", 30, 130.0, 1045);
    }

    sf::Text down;
    if(speedFactor == 12)
    {
        createText(down, font_1, "MIN", 18, 60.0, 1048);
    }
    else
    {
        createText(down, font_1, "^", 30, 85.0, 1075);
        down.rotate(180.0);
    }

    _window.draw(title);
    _window.draw(closest);
    _window.draw(passenger);
    _window.draw(miles);
    _window.draw(timeText);
    _window.draw(dfs);
    _window.draw(bfs);
    _window.draw(reset);
    _window.draw(restart);
    _window.draw(vis);
    _window.draw(rankList);
    _window.draw(stateList);
    _window.draw(passList);
    _window.draw(mileList);
    _window.draw(visSpeed);
    _window.draw(up);
    _window.draw(down);
}
void createText(sf::Text& obj, sf::Font& font, string text, int size, float x, float y)
{
    obj.setFont(font);
    obj.setString(text);
    obj.setCharacterSize(size);
    obj.setFillColor(sf::Color::Black);
    obj.setPosition(x, y);
}
