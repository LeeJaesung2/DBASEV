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
        {1, 8, 35.1534523f, 128.1006654f, 5},
        {1, 9, 35.1534447f, 128.1007197f, 5},
        {1, 10, 35.1534351f, 128.100773f, 5},
        {1, 11, 35.1534271f, 128.1008267f, 5},
        {1, 12, 35.1534208f, 128.1008813f, 5},
        {1, 13, 35.1534126f, 128.100935f, 5},
        {1, 14, 35.1534038f, 128.1009893f, 5},
        {1, 15, 35.1533948f, 128.1010436f, 5},
        {1, 16, 35.1533868f, 128.1010972f, 5},
        {1, 17, 35.1533789f, 128.1011509f, 5},
        {1, 18, 35.1533701f, 128.1012052f, 5},
        {1, 19, 35.1533613f, 128.1012592f, 5},
        {1, 20, 35.1533517f, 128.1013132f, 5},
        {1, 21, 35.1533446f, 128.1013675f, 5},
        {1, 22, 35.1533369f, 128.1014218f, 5},
        {1, 23, 35.1533298f, 128.1014758f, 5},
        {1, 24, 35.153321f, 128.1015294f, 5},
        {1, 25, 35.1533134f, 128.1015834f, 5},
        {1, 26, 35.1533043f, 128.1016374f, 5},
        {1, 27, 35.1532961f, 128.101691f, 5},
        {1, 28, 35.1532879f, 128.1017447f, 5},
        {1, 29, 35.1532796f, 128.1017993f, 5},
        {1, 30, 35.15327f, 128.1018529f, 5},
        {1, 31, 35.1532618f, 128.1019066f, 5},
        {1, 32, 35.1532544f, 128.1019609f, 5},
        {1, 33, 35.1532465f, 128.1020152f, 5},
        {1, 34, 35.1532388f, 128.1020695f, 5},
        {1, 35, 35.1532308f, 128.1021232f, 5},
        {1, 36, 35.1532218f, 128.1021772f, 5},
        {1, 37, 35.1532119f, 128.1022452f, 5},
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
        #ifndef DEBUG
            cout << "Map.cpp" << endl;
        #endif
        cout << "Waypoints: ";
        for (int waypointId : vertex.waypoints) {
            cout << waypointId << " ";
        }
        cout << endl;
        #ifndef DEBUG
            cout << "Map.cpp" << endl;
        #endif
        cout << "Connected Roads: ";
        for (int connectedRoad : vertex.connectedRoads) {
            cout << connectedRoad << " ";
        }
        cout << endl << endl;
        #ifndef DEBUG
            cout << "Map.cpp" << endl;
        #endif
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

// Function to calculate the closest waypoint
int calculateClosestWaypoint(int road_id, int pre_waypoint, float current_latitude, float current_longitude, const unordered_map<int, Vertex>& graph) {
    vector<int> waypoints = getWaypoints(graph, road_id);
    int now_waypoint = waypoints[0];

    const vector<float>& waypoint_latitude = graph.at(road_id).latitude;
    const vector<float>& waypoint_longitude = graph.at(road_id).longitude;

    float min_distance = numeric_limits<float>::max();

    for (int i = pre_waypoint; i < static_cast<int>(waypoints.size()); i++) {
        float distance = calc_distance(current_latitude, current_longitude, waypoint_latitude[i], waypoint_longitude[i]);

        cout << "waypoint " << waypoints[i] << " distance: " << distance << endl;

        if (distance > min_distance) {
            break;
        }

        min_distance = distance;
        now_waypoint = i;
    }

    return now_waypoint;
}

int findNextRoadId(int road_id, float current_latitude, float current_longitude, const unordered_map<int, Vertex> &graph)
{
    int next_road_id;

    // Get the latitude and longitude of the first waypoint of all adjacent road_ids connected to the current road_id.
    vector<int> waypoints = getWaypoints(graph, road_id);
    vector<int> connected_roads = graph.at(road_id).connectedRoads;
    float min_distance = numeric_limits<float>::max();
    int nearest_road_id = -1;
    vector<float> first_waypoint_coordinates;

    for (int connected_road : connected_roads)
    {
        vector<int> connected_waypoints = getWaypoints(graph, connected_road);
        if (!connected_waypoints.empty())
        {
            int first_waypoint_id = connected_waypoints[0];

            cout << " first_waypoint_id: " << first_waypoint_id << " of road: " << connected_road;

            float waypoint_latitude = graph.at(connected_road).latitude[0];
            float waypoint_longitude = graph.at(connected_road).longitude[0];

            // Measure the distance between the two GPS values.
            float distance = calc_distance(current_latitude, current_longitude, waypoint_latitude, waypoint_longitude);
            if (distance < min_distance)
            {
                min_distance = distance;
                nearest_road_id = connected_road;
            }

            cout << "     distance: " << distance << "\n";
        }
    }

    // Check if the distance is valid and if it is within 3 meters, update the next_road_id to the nearest road_id.
    if (nearest_road_id != -1 && min_distance <= 5)
    {
        next_road_id = nearest_road_id;
        waypoints = getWaypoints(graph, road_id);
    }

    return next_road_id;
}