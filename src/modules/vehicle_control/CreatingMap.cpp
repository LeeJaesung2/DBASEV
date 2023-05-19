#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

// 그래프 정점 구조체
struct Vertex {
    int id;
    vector<int> waypoints;
    vector<int> connectedRoads;
};

// 도로 연결 함수
void connectRoads(unordered_map<int, Vertex>& graph, int roadId1, int roadId2) {
    // roadId1과 roadId2를 서로 연결
    graph[roadId1].connectedRoads.push_back(roadId2);
    graph[roadId2].connectedRoads.push_back(roadId1);
}

void creatingMap() {
    unordered_map<int, Vertex> graph;  // 그래프 저장을 위한 해시맵

    // 엑셀 파일 데이터
    vector<vector<double>> excelData = {
        {1, 1, 35.1535098, 128.1002861, 5},
        {1, 2, 35.1535017, 128.1003405, 5},
        {1, 3, 35.153494, 128.1003952, 5},
        {1, 4, 35.1534847, 128.1004491, 5},
        {1, 5, 35.153477, 128.1005038, 5},
        {1, 6, 35.1534693, 128.1005578, 5},
        {1, 7, 35.1534608, 128.1006118, 5},
        {2, 38, 35.1532566, 128.1022536, 5},
        {2, 39, 35.153301, 128.102264, 5},
        {2, 40, 35.1533454, 128.1022747, 5},
        {3, 79, 35.1550369, 128.1026779, 5},
        {3, 80, 35.1550296, 128.102732, 5},
        {3, 81, 35.1550217, 128.1027867, 5},
        {3, 82, 35.1550144, 128.1028408, 5},
        {3, 83, 35.1550073, 128.1028952, 5},
        {3, 84, 35.1549995, 128.1029496, 5}
        // ... 나머지 데이터
    };

    // 엑셀 파일 데이터를 그래프로 변환
    for (const auto& row : excelData) {
        int roadId = row[0];
        int waypointId = row[1];
        int lat = row[2];
        int lon = row[3];
        int alt = row[4];

        // 도로 ID가 그래프에 없으면 새로운 정점 추가
        if (graph.find(roadId) == graph.end()) {
            Vertex vertex;
            vertex.id = roadId;
            graph[roadId] = vertex;
        }

        // 해당 도로의 정점에 웨이포인트 ID 추가
        graph[roadId].waypoints.push_back(waypointId);
    }

    // 도로 연결
    connectRoads(graph, 1, 2);
    connectRoads(graph, 2, 3);

    // 그래프 정보 출력
    for (const auto& pair : graph) {
        int roadId = pair.first;
        const Vertex& vertex = pair.second;
        cout << "도로 ID: " << roadId << endl;
        cout << "웨이포인트: ";
        for (int waypointId : vertex.waypoints) {
            cout << waypointId << " ";
        }
        cout << endl;
        cout << "연결된 도로: ";
        for (int connectedRoad : vertex.connectedRoads) {
            cout << connectedRoad << " ";
        }
        cout << endl << endl;
    }
}
