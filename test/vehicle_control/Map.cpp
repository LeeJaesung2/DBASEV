#include "./Map.h"

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
        {2, 41, 35.1533898f, 128.1022828f, 5},
        {2, 42, 35.153434f, 128.1022915f, 5},
        {2, 43, 35.1534926f, 128.1023035f, 5},
        {2, 44, 35.1535368f, 128.102314f, 5},
        {2, 45, 35.1535815f, 128.1023233f, 5},
        {2, 46, 35.1536261f, 128.1023334f, 5},
        {2, 47, 35.15367f, 128.1023431f, 5},
        {2, 48, 35.1537144f, 128.1023515f, 5},
        {2, 49, 35.1537583f, 128.1023609f, 5},
        {2, 50, 35.1538027f, 128.1023696f, 5},
        {2, 51, 35.1538468f, 128.1023787f, 5},
        {2, 52, 35.1538912f, 128.1023877f, 5},
        {2, 53, 35.1539354f, 128.1023958f, 5},
        {2, 54, 35.15398f, 128.1024035f, 5},
        {2, 55, 35.1540247f, 128.1024129f, 5},
        {2, 56, 35.1540688f, 128.1024226f, 5},
        {2, 57, 35.1541135f, 128.1024316f, 5},
        {2, 58, 35.1541577f, 128.1024409f, 5},
        {2, 59, 35.1542017f, 128.1024499f, 5},
        {2, 60, 35.1542463f, 128.1024596f, 5},
        {2, 61, 35.1542909f, 128.1024685f, 5},
        {2, 62, 35.1543353f, 128.1024776f, 5},
        {2, 63, 35.1543796f, 128.1024861f, 5},
        {2, 64, 35.1544236f, 128.1024958f, 5},
        {2, 65, 35.154468f, 128.1025049f, 5},
        {2, 66, 35.1545124f, 128.1025143f, 5},
        {2, 67, 35.1545572f, 128.1025233f, 5},
        {2, 68, 35.1546011f, 128.1025331f, 5},
        {2, 69, 35.1546453f, 128.1025419f, 5},
        {2, 70, 35.1546893f, 128.1025507f, 5},
        {2, 71, 35.154734f, 128.1025594f, 5},
        {2, 72, 35.1547781f, 128.1025684f, 5},
        {2, 73, 35.1548221f, 128.1025776f, 5},
        {2, 74, 35.1548663f, 128.1025862f, 5},
        {2, 75, 35.1549108f, 128.1025961f, 5},
        {2, 76, 35.1549556f, 128.102605f, 5},
        {2, 77, 35.1549996f, 128.102615f, 5},
        {2, 78, 35.155044f, 128.1026237f, 5},
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