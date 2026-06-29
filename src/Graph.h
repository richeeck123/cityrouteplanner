#pragma once
#include <unordered_map>
#include <vector>
#include <cmath>

using namespace std;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct Node {
    long long id;
    double lat, lon;
};

double haversine(double lat1, double lon1, double lat2, double lon2) {
    const double R = 6371.0;
    double dLat = (lat2 - lat1) * M_PI/180.0;
    double dLon = (lon2 - lon1) * M_PI/180.0;
    double a = sin(dLat/2) * sin(dLat/2) +
               cos(lat1 * M_PI/180.0)*cos(lat2 * M_PI/180.0)*
               sin(dLon/2) * sin(dLon/2);
    return 2*R*atan2(sqrt(a), sqrt(1 - a));
}

class Graph {
public:
    unordered_map<long long, Node> nodes;
    unordered_map<long long, vector<pair<long long, double>>> adj;

    void addNode(long long id,double lat,double lon) {
        nodes[id] = {id, lat, lon};
    }

    void addEdge(long long u, long long v) {
        if (nodes.find(u) == nodes.end() || nodes.find(v) == nodes.end()) return;
        double dist = haversine(
            nodes[u].lat,
            nodes[u].lon,
            nodes[v].lat,
            nodes[v].lon
        );
        adj[u].push_back({v, dist});
        adj[v].push_back({u, dist});
    }
};