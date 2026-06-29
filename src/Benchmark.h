#pragma once
#include "Algorithms.h"
#include <chrono>
#include <vector>
#include <iostream>
#include <iomanip>
using namespace std;

#define ll long long

class BenchResult {
public:
    double distKm;
    ll nodesVisited;
    ll timeUs;
    vector<ll> path;
    BenchResult() {}
    BenchResult(double distKm, ll nodesVisited, ll timeUs, vector<ll> path){
        this->distKm = distKm;
        this->nodesVisited = nodesVisited;
        this->timeUs = timeUs;
        this->path = move(path);
    }
};
class Benchmark {
public:
    static BenchResult runDijkstra(Graph &g, ll src, ll dst) {
        ll visited = 0;
        auto start = chrono::high_resolution_clock::now();
        auto [dist, path] = dijkstra(g, src, dst, &visited);
        auto end = chrono::high_resolution_clock::now();
        ll us = chrono::duration_cast<chrono::microseconds>(end - start).count();
        return BenchResult(dist, visited, us, move(path));
    }
    static BenchResult runAStar(Graph &g, ll src, ll dst) {
        ll visited = 0;
        auto start = chrono::high_resolution_clock::now();
        auto [dist, path] = astar(g, src, dst, &visited);
        auto end = chrono::high_resolution_clock::now();
        ll us = chrono::duration_cast<chrono::microseconds>(end - start).count();
        return BenchResult(dist, visited, us, move(path));
    }
    static void printComparison(BenchResult &d, BenchResult &a) {
        cout << fixed << setprecision(2);
        cout << "\nDijkstra: " << d.distKm << "km | " << d.nodesVisited << " nodes | " << d.timeUs << "us\n";
        cout << "A*:       " << a.distKm << "km | " << a.nodesVisited << " nodes | " << a.timeUs << "us\n";
        if (a.nodesVisited) cout << "A* speedup: " << (double)d.nodesVisited / a.nodesVisited << "x fewer nodes visited\n";
        else cout << "A* speedup: N/A\n";
    }
};