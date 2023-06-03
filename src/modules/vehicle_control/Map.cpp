#include <DBASEV/Map.h>
#include <DBASEV/MapData.h>

using namespace std;

// Connect roads function
void connectRoads(unordered_map<int, Vertex>& graph, int roadId1, int roadId2) {
    // Connect roadId1 and roadId2
    graph[roadId1].connectedRoads.push_back(roadId2);
    graph[roadId2].connectedRoads.push_back(roadId1);
}

unordered_map<int, Vertex> creatingMap() {
    unordered_map<int, Vertex> graph; // Hashmap to store the graph

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
    //connectRoads(graph, 2, 3);

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
    int now_waypoint = pre_waypoint;

    const vector<float>& waypoint_latitude = graph.at(road_id).latitude;
    const vector<float>& waypoint_longitude = graph.at(road_id).longitude;

    float min_distance = numeric_limits<float>::max();

    for (int i = now_waypoint; i < static_cast<int>(waypoints.size()); i++) {
        float distance = calc_distance(current_latitude, current_longitude, waypoint_latitude[i], waypoint_longitude[i]);

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
            //cout << " first_waypoint_id: " << first_waypoint_id << " of road: " << connected_road;

            float waypoint_latitude = graph.at(connected_road).latitude[0];
            float waypoint_longitude = graph.at(connected_road).longitude[0];

            // Measure the distance between the two GPS values.
            float distance = calc_distance(current_latitude, current_longitude, waypoint_latitude, waypoint_longitude);
            if (distance < min_distance)
            {
                min_distance = distance;
                nearest_road_id = connected_road;
            }

            //cout << "     distance: " << distance << "\n";
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