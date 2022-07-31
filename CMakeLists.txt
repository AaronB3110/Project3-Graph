cmake_minimum_required(VERSION 3.21)
project(Project_3___Graph_and_Flights)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Project_3___Graph_and_Flights
        "Flights Final CSV.csv"
        graph.cpp
        graph.h
        main.cpp
        StateCodes.csv)
