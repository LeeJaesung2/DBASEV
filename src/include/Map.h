#pragma once
#ifndef MAP_H
#define MAP_H

#include <DBASEV/visibility.h>
#include <unordered_map>

using namespace std;

struct Vertex {
    int id;
    std::vector<int> waypoints;
    std::vector<int> connectedRoads;
};

unordered_map<int, Vertex> creatingMap();

vector<int> getWaypoints(const unordered_map<int, Vertex>& graph, int roadId);

#endif // MAP_H