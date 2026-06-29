#pragma once
#include "Graph.h"
#include <fstream>
#include <string>
#include <vector>

using namespace std;

long long extractLongLong(string line, string attr){
    string key = attr + "=\"";
    int pos = line.find(key) + key.size();
    int end = line.find("\"", pos);
    return stoll(line.substr(pos, end - pos));
}

double extractDouble(string line, string attr){
    string key = attr + "=\"";
    int pos = line.find(key) + key.size();
    int end = line.find("\"", pos);
    return stod(line.substr(pos, end - pos));
}

Graph parseOSM(string filename) {
    Graph g;
    ifstream file(filename);
    string line;
    vector<long long> wayNodes;
    bool isHighway = false;
    while (getline(file, line)) {
        if (line.find("<node ") != string::npos) {
            long long id = extractLongLong(line, "id");
            double lat = extractDouble(line, "lat");
            double lon = extractDouble(line, "lon");
            g.addNode(id, lat, lon);
        }

        else if (line.find("<way ") != string::npos) {
            wayNodes.clear();
            isHighway = false;
        }
        else if (line.find("<nd ref=") != string::npos) {
            long long id = extractLongLong(line, "ref");
            wayNodes.push_back(id);
        }
        else if (line.find("k=\"highway\"") != string::npos) {
            isHighway = true;
        }
        else if (line.find("</way>") != string::npos) {
            if (!isHighway) continue;
            for (int i=0;i+1<wayNodes.size();i++) {
                g.addEdge(wayNodes[i], wayNodes[i + 1]);
            }
        }
    }
    return g;
}