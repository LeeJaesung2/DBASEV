#pragma once
#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

struct Vertex {
    int id;
    vector<int> waypoints;
    vector<int> connectedRoads;
};

unordered_map<int, Vertex> creatingMap();

vector<int> getWaypoints(const unordered_map<int, Vertex>& graph, int roadId);

#endif // MAP_H
