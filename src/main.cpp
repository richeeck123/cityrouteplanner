#include "Graph.h"
#include "Parser.h"
#include "Algorithms.h"
#include "Benchmark.h"
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

void exportPath(Graph &g,const vector<long long> &path,double dist,const string &algo){

    ofstream f("output/path_output.json");
    if (!f.is_open()) {
        cout << "Could not open output file.\n";
        return;
    }
    f << "{\n";
    f << "  \"algo\": \"" << algo << "\",\n";
    f << "  \"dist_km\": " << dist << ",\n";
    f << "  \"path\": [\n";
    for (int i = 0; i < path.size(); i++) {
        Node node = g.nodes[path[i]];
        f << "    ["<< node.lat << ", "<< node.lon << "]";
        if (i != path.size() - 1) f << ",";
        f << "\n";
    }
    f << "  ]\n";
    f << "}\n";
}

int main() {
    cout << "City Route Planner\n";
    cout << "Loading map...\n";
    Graph g = parseOSM("data/city.osm");
    if (g.nodes.empty()) {
        cout << "Failed to load map.\n";
        return 0;
    }
    long long edges = 0;
    for (auto &it : g.adj) edges += it.second.size();
    edges /= 2;
    cout << "Nodes : " << g.nodes.size() << endl;
    cout << "Edges : " << edges << endl;

    double srcLat, srcLon;
    double dstLat, dstLon;
    cout << "\nSource Latitude : ";
    cin >> srcLat;
    cout << "Source Longitude: ";
    cin >> srcLon;
    cout << "Destination Latitude : ";
    cin >> dstLat;
    cout << "Destination Longitude: ";
    cin >> dstLon;
    cout << "\nFinding nearest nodes...\n";

    long long src = nearestNode(g, srcLat, srcLon);
    long long dst = nearestNode(g, dstLat, dstLon);
    if (src == -1 || dst == -1) {
        cout << "Invalid coordinates.\n";
        return 0;
    }
    cout << "Running algorithms...\n\n";

    BenchResult d = Benchmark::runDijkstra(g, src, dst);
    BenchResult a = Benchmark::runAStar(g, src, dst);
    Benchmark::printComparison(d, a);

    if (!a.path.empty()) {
        exportPath(g,a.path,a.distKm,"A*");
        cout << "\nPath exported to output/path_output.json\n";
    }
    else if (!d.path.empty()) {
        exportPath(g,d.path,d.distKm,"Dijkstra");
        cout << "\nPath exported to output/path_output.json\n";
    }
    else {
        cout << "\nNo path found.\n";
    }
    return 0;
}