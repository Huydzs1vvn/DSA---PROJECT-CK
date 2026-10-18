#include <iostream>
#include <vector>
#include <string>
#include <stack>

using namespace std;

// Graph structure using Adjacency List
struct Graph {
    int V;                              // Number of vertices
    bool directed;                      // Directed (true) or undirected (false)
    vector<vector<int>> adj;            // Adjacency list
    vector<string> names;               // Vertex labels ("A", "B", "C", ...)

    Graph(const vector<string>& vertexNames, bool isDirected = true) {
        V = vertexNames.size();
        directed = isDirected;
        names = vertexNames;
        adj.resize(V);
    }

    // Get vertex integer index from label
    int getIndex(const string& name) {
        for (int i = 0; i < V; ++i) {
            if (names[i] == name) return i;
        }
        return -1;
    }

    // Add an edge between two vertices
    void addEdge(const string& uName, const string& vName) {
        int u = getIndex(uName);
        int v = getIndex(vName);
        if (u != -1 && v != -1) {
            adj[u].push_back(v);
            if (!directed) {
                adj[v].push_back(u); // Add reverse edge for undirected graph
            }
        }
    }

    // Print adjacency list representation
    void printGraph() {
        cout << "Adjacency List (" << (directed ? "Directed" : "Undirected") << "):\n";
        for (int u = 0; u < V; ++u) {
            cout << "  " << names[u] << " -> ";
            for (size_t i = 0; i < adj[u].size(); ++i) {
                cout << names[adj[u][i]] << (i + 1 < adj[u].size() ? ", " : "");
            }
            cout << "\n";
        }
    }

    // Iterative DFS + Cycle Detection using inStack array
    bool iterativeDFS(const string& startName, vector<string>& order, vector<pair<string, string>>& backEdges) {
        int start = getIndex(startName);
        if (start == -1) return false;

        // visited[u]: Whether vertex u has been discovered
        vector<bool> visited(V, false);
        // inStack[u]: Whether vertex u is on the active search path in stack
        vector<bool> inStack(V, false);
        vector<int> parent(V, -1);
        bool hasCycle = false;

        // Stack storing frames: (current vertex u, next edge index to inspect)
        stack<pair<int, int>> st;

        // Initialize root vertex
        visited[start] = true;
        inStack[start] = true;
        order.push_back(names[start]);
        st.push({start, 0});

        while (!st.empty()) {
            int u = st.top().first;
            int& i = st.top().second; // Reference to track edge index across backtracking

            // If vertex u still has unexplored adjacent edges
            if (i < (int)adj[u].size()) {
                int v = adj[u][i++]; // Fetch neighbor and advance index

                // In undirected graphs, ignore trivial back-edge to direct parent
                if (!directed && v == parent[u]) continue;

                if (!visited[v]) {
                    // Tree Edge: vertex v is undiscovered -> push to stack, mark inStack
                    parent[v] = u;
                    visited[v] = true;
                    inStack[v] = true;
                    order.push_back(names[v]);
                    st.push({v, 0});
                } 
                else if (inStack[v]) {
                    // Back-Edge: vertex v is currently in stack (ancestor) -> CYCLE DETECTED!
                    hasCycle = true;
                    backEdges.push_back({names[u], names[v]});
                }
                // If visited[v] == true but inStack[v] == false:
                // Vertex v belongs to a fully finished branch (Cross/Forward edge), not a cycle
            } 
            else {
                // All neighbors of u explored -> Pop from stack (Backtrack)
                inStack[u] = false;
                st.pop();
            }
        }

        return hasCycle;
    }
};

// Main function: execute unit tests
int main() {
    cout << "DSA - ITERATIVE DFS & CYCLE DETECTION (inStack)\n\n";

    // TEST 1: Replicate Week 1 6-node graph with cycle (A, B, C, D, E, F)
    cout << "[TEST 1] Week 1 6-Vertex Graph (Contains Cycle):\n";
    Graph g({"A", "B", "C", "D", "E", "F"}, /*directed=*/true);
    g.addEdge("A", "B");
    g.addEdge("A", "C");
    g.addEdge("B", "D");
    g.addEdge("D", "B"); // Back-edge D -> B creates a cycle!
    g.addEdge("D", "E");
    g.addEdge("E", "F");
    g.addEdge("C", "F");

    g.printGraph();

    vector<string> order;
    vector<pair<string, string>> backEdges;
    bool hasCycle = g.iterativeDFS("A", order, backEdges);

    cout << "\n-> DFS Discovery Order: ";
    for (size_t i = 0; i < order.size(); ++i) {
        cout << order[i] << (i + 1 < order.size() ? " -> " : "");
    }
    cout << "\n-> Cycle Detected    : " << (hasCycle ? "YES" : "NO") << "\n";
    for (auto edge : backEdges) {
        cout << "   * Back-edge closing cycle: " << edge.first << " -> " << edge.second << "\n";
    }

    // TEST 2: Directed Acyclic Graph (DAG) - Verify no false positives
    cout << "\n[TEST 2] Directed Acyclic Graph (DAG - Verify No False Cycle):\n";
    Graph dag({"A", "B", "C", "D"}, /*directed=*/true);
    dag.addEdge("A", "B");
    dag.addEdge("A", "C");
    dag.addEdge("B", "D");
    dag.addEdge("C", "D"); // Cross edge to D (inStack[D] == false), not a cycle

    vector<string> dagOrder;
    vector<pair<string, string>> dagBackEdges;
    bool dagCycle = dag.iterativeDFS("A", dagOrder, dagBackEdges);

    cout << "-> Cycle Detected    : " << (dagCycle ? "YES" : "NO") << " (Correct!)\n\n";
}
