#pragma once
#include "Graph.h"
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>

using namespace std;

pair<double, vector<long long>> dijkstra(Graph &g,long long src,
    long long dst,long long *nodesVisited = nullptr){

    unordered_map<long long, double> dist;
    unordered_map<long long, long long> prev;
    priority_queue<pair<double, long long>,vector<pair<double, long long>>,
        greater<pair<double, long long>>> pq;

    for (auto &it : g.nodes) dist[it.first] = 1e18;

    dist[src] = 0;
    pq.push({0, src});
    long long visited = 0;

    while (!pq.empty()) {
        double curDist = pq.top().first;
        long long u = pq.top().second;
        pq.pop();
        visited++;
        if (u == dst) break;
        if (curDist > dist[u]) continue;
        if (g.adj.find(u) == g.adj.end()) continue;
        for (auto edge : g.adj[u]) {
            long long v = edge.first;
            double w = edge.second;
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                prev[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
    if (nodesVisited) *nodesVisited = visited;
    if (dist[dst] >= 1e17) return {1e18, {}};
    vector<long long> path;
    long long cur = dst;
    while (cur != src){
        if (prev.find(cur) == prev.end()) return {1e18, {}};
        path.push_back(cur);
        cur = prev[cur];
    }
    path.push_back(src);
    reverse(path.begin(), path.end());
    return {dist[dst], path};
}

double heuristic(Graph &g,long long u,long long dst) {
    return haversine(
        g.nodes[u].lat,
        g.nodes[u].lon,
        g.nodes[dst].lat,
        g.nodes[dst].lon
    );
}

pair<double, vector<long long>> astar(Graph &g, long long src,
    long long dst, long long *nodesVisited = nullptr){

    unordered_map<long long, double> gScore;
    unordered_map<long long, long long> prev;
    priority_queue<pair<double, long long>,vector<pair<double, long long>>,
        greater<pair<double, long long>>> pq;

    for (auto &it : g.nodes) gScore[it.first] = 1e18;

    gScore[src] = 0;
    pq.push({heuristic(g, src, dst), src});
    long long visited = 0;

    while (!pq.empty()) {
        long long u = pq.top().second;
        pq.pop();
        visited++;
        if(u == dst) break;
        if (g.adj.find(u) == g.adj.end()) continue;
        for (auto edge : g.adj[u]){
            long long v = edge.first;
            double w = edge.second;
            double newDist = gScore[u] + w;
            if(newDist < gScore[v]){
                gScore[v] = newDist;
                prev[v] = u;
                pq.push({newDist + heuristic(g, v, dst), v});
            }
        }
    }

    if (nodesVisited) *nodesVisited = visited;
    if (gScore[dst] >= 1e17) return {1e18, {}};
    vector<long long> path;
    long long cur = dst;
    while (cur != src) {
        if (prev.find(cur) == prev.end()) return {1e18, {}};
        path.push_back(cur);
        cur = prev[cur];
    }
    path.push_back(src);
    reverse(path.begin(), path.end());
    return {gScore[dst], path};
}

vector<long long> bfs(Graph &g,long long src,long long dst){

    unordered_map<long long, long long> prev;
    unordered_set<long long> vis;
    queue<long long> q;

    q.push(src);
    vis.insert(src);

    while (!q.empty()) {
        long long u = q.front();
        q.pop();
        if (u == dst) break;
        if (g.adj.find(u) == g.adj.end()) continue;
        for (auto edge : g.adj[u]){
            long long v = edge.first;
            if (!vis.count(v)){
                vis.insert(v);
                prev[v] = u;
                q.push(v);
            }
        }
    }

    vector<long long> path;
    if (src != dst && prev.find(dst) == prev.end()) return path;
    long long cur = dst;
    while (cur != src) {
        path.push_back(cur);
        cur = prev[cur];
    }
    path.push_back(src);
    reverse(path.begin(), path.end());
    return path;
}

long long nearestNode(Graph &g,double lat,double lon) {

    long long bestNode = -1;
    double bestDist = 1e18;
    for (auto &it : g.nodes) {
        long long id = it.first;
        if (g.adj.find(id) == g.adj.end()) continue;
        double dist = haversine(
            lat,
            lon,
            it.second.lat,
            it.second.lon
        );
        if (dist < bestDist) {
            bestDist = dist;
            bestNode = id;
        }
    }
    return bestNode;
}