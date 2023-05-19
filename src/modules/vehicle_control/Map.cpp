#include <DBASEV/Map.h>

using namespace std;

// Connect roads function
void connectRoads(unordered_map<int, Vertex>& graph, int roadId1, int roadId2) {
    // Connect roadId1 and roadId2
    graph[roadId1].connectedRoads.push_back(roadId2);
    graph[roadId2].connectedRoads.push_back(roadId1);
}

unordered_map<int, Vertex> creatingMap() {
    unordered_map<int, Vertex> graph; // Hashmap to store the graph

    // Excel file data
    vector<vector<float>> excelData = {
        {1, 1, 35.1535098f, 128.1002861f, 5},
        {1, 2, 35.1535017f, 128.1003405f, 5},
        {1, 3, 35.153494f, 128.1003952f, 5},
        {1, 4, 35.1534847f, 128.1004491f, 5},
        {1, 5, 35.153477f, 128.1005038f, 5},
        {1, 6, 35.1534693f, 128.1005578f, 5},
        {1, 7, 35.1534608f, 128.1006118f, 5},
        {2, 38, 35.1532566f, 128.1022536f, 5},
        {2, 39, 35.153301f, 128.102264f, 5},
        {2, 40, 35.1533454f, 128.1022747f, 5},
        {3, 79, 35.1550369f, 128.1026779f, 5},
        {3, 80, 35.1550296f, 128.102732f, 5},
        {3, 81, 35.1550217f, 128.1027867f, 5},
        {3, 82, 35.1550144f, 128.1028408f, 5},
        {3, 83, 35.1550073f, 128.1028952f, 5},
        {3, 84, 35.1549995f, 128.1029496f, 5}
        // ... Remaining data
    };

    // Convert Excel file data to the graph
    for (const auto& row : excelData) {
        int roadId = row[0];
        int waypointId = row[1];
        float lat = row[2];
        float lon = row[3];
        //int alt = row[4];

        // Add a new vertex if the road ID doesn't exist in the graph
        if (graph.find(roadId) == graph.end()) {
            Vertex vertex;
            vertex.id = roadId;
            graph[roadId] = vertex;
        }

        // Add waypoint ID to the corresponding road's vertex
        graph[roadId].waypoints.push_back(waypointId);
        graph[roadId].latitude.push_back(lat);
        graph[roadId].longitude.push_back(lon);
    }

    // Connect roads
    connectRoads(graph, 1, 2);
    connectRoads(graph, 2, 3);

    // Print graph information
    for (const auto& pair : graph) {
        int roadId = pair.first;
        const Vertex& vertex = pair.second;
        cout << "Road ID: " << roadId << endl;
        cout << "Waypoints: ";
        for (int waypointId : vertex.waypoints) {
            cout << waypointId << " ";
        }
        cout << endl;
        cout << "Connected Roads: ";
        for (int connectedRoad : vertex.connectedRoads) {
            cout << connectedRoad << " ";
        }
        cout << endl << endl;
    }

    return graph;
}

vector<int> getWaypoints(const unordered_map<int, Vertex>& graph, int roadId) {
    vector<int> waypoints;

    if (graph.find(roadId) != graph.end()) {
        waypoints = graph.at(roadId).waypoints;
    }

    return waypoints;
}