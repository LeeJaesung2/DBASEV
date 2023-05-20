#pragma once
#ifndef MAP_H
#define MAP_H

#include <DBASEV/visibility.h>
#include <unordered_map>
#include <limits>

#include "GPS_functions.h"

using namespace std;

struct Vertex {
    int id;
    vector<int> waypoints;
    vector<int> connectedRoads;
    vector<float> latitude;  
    vector<float> longitude;
};

unordered_map<int, Vertex> creatingMap();

vector<int> getWaypoints(const unordered_map<int, Vertex>& graph, int roadId);

// Function to calculate the closest waypoint
int calculateClosestWaypoint(int road_id, int pre_waypoint, float current_latitude, float current_longitude, const unordered_map<int, Vertex>& graph);

int findNextRoadId(int road_id, float current_latitude, float current_longitude, const unordered_map<int, Vertex> &graph);

#endif // MAP_H