# City Route Planner

City Route Planner is a high-performance C++ backend application paired with a modern, responsive HTML5/JS Leaflet dashboard. The system parses OpenStreetMap (OSM) XML data, builds a weighted, undirected spatial graph representation of a city's road network, executes shortest-path routing algorithms, profiles execution performance, and renders routes on an interactive web interface.

The application serves as a benchmark utility comparing **Dijkstra's Algorithm** and **A* Search** on real-world spatial structures, demonstrating how heuristic-driven exploration reduces search spaces and computation times.

---

## Key Features

- **OSM XML Parser**: Extract spatial nodes and highway connections directly from OpenStreetMap data.
- **Weighted Graph Storage**: Build adjacency list representations where intersections are vertices and road segments are edges weighted by physical distance.
- **Advanced Pathfinding**:
  - **Dijkstra's Algorithm**: Guarantees optimal pathing using a min-heap priority queue.
  - **A* Search**: Guides exploration toward the destination using the Haversine formula as a consistent heuristic.
  - **Breadth-First Search (BFS)**: Traverses nodes level-by-level as an unweighted comparison.
- **Microsecond Benchmarking**: Measures and compares runtime durations and total search nodes explored.
- **Full-Screen Interactive Map Dashboard**:
  - Light/Dark map themes using CartoDB tile layers.
  - Path rendering with custom start and destination markers.
  - Click-to-copy coordinates utility to easily capture inputs for the C++ terminal.


---

## System Architecture

The project consists of a decoupled C++ backend and a web-based frontend:

```
[ OSM Map Data (city.osm) ]
             │
             ▼
      [ Parser.h ]
             │
             ▼
      [ Graph.h ] <─── [ Algorithms.h ] ───> [ Benchmark.h ]
             │
             ▼
    [ main.cpp (Driver) ]
             │
             ▼
[ Route Path JSON (path_output.json) ]
             │
             ▼
    [ map.html (Web Dashboard) ]
```

---


## Algorithm Details

### 1. Haversine Distance
The geodesic distance between two spherical coordinates is calculated as:
$$d = 2 \cdot R \cdot \arcsin\left(\sqrt{\sin^2\left(\frac{\Delta \text{lat}}{2}\right) + \cos(\text{lat}_1) \cdot \cos(\text{lat}_2) \cdot \sin^2\left(\frac{\Delta \text{lon}}{2}\right)}\right)$$
Where $R = 6371.0\text{ km}$. This distance is used as edge weights in the graph and serves as the consistent heuristic for A* Search.

### 2. Dijkstra's Algorithm
Finds shortest paths in weighted graphs. It relaxes node distances outward from the source using a priority queue.
- **Time Complexity**: $O((V + E) \log V)$
- **Space Complexity**: $O(V)$

### 3. A* Search
Implements heuristic-driven pathfinding. It relaxes nodes prioritizing those minimizing:
$$f(n) = g(n) + h(n)$$
Where $g(n)$ is the cost from the start, and $h(n)$ is the Haversine distance to the destination. Since $h(n)$ is admissible (never overestimates distance), the path is guaranteed to be optimal while checking up to 90% fewer nodes.
- **Time Complexity**: $O((V + E) \log V)$ (practically much faster than Dijkstra)
- **Space Complexity**: $O(V)$

### 4. Breadth-First Search (BFS)
Explores nodes level-by-level, assuming all edge weights are $1.0$. Used as a baseline unweighted control.
- **Time/Space Complexity**: $O(V + E)$ / $O(V)$

---

## Building and Running the C++ App

### Compilation
Compile the application using a C++17 compatible compiler:
```bash
g++ -std=c++17 src/main.cpp -Isrc -o planner
```

### Execution
Run the compiled executable:
```bash
./planner
```

Upon launching, the app will:
1. Parse the OSM map data file (`data/city.osm`).
2. Display the total nodes and edges resolved.
3. Prompt you to enter coordinates (Latitude/Longitude) for the source and destination.
4. Execute Dijkstra's and A* search algorithms.
5. Print performance comparison stats and export the resulting path coordinates to `output/path_output.json`.

---

## Viewing the Interactive Web Dashboard

To visualize the computed route on the map:

1. Launch a local web server from the project directory:
   ```bash
   python -m http.server 8000
   ```
2. Open your browser and navigate to:
   ```text
   http://localhost:8000/map.html
   ```

### Dashboard Features
- **Floating Panel**: View the path metrics (distance in km and active algorithm).
- **Theme Switcher**: Tap the theme toggle button in the header to switch between Light (`CartoDB Positron`) and Dark (`CartoDB Dark Matter`) visual layers.
- **Coordinate Picker**: Click anywhere on the map to display its latitude/longitude. Click **Copy Coordinates** to copy a copy-pasteable format (e.g., `18.9219 72.8347`) to insert directly into your running C++ console planner application.

---
