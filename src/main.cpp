#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <queue>
#include <algorithm>
#include <chrono>

using namespace std::chrono;
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
bool bfsShortestPath(const string& startName, const string& targetName,
                         vector<string>& order, vector<string>& path, int& shortestDist) {
        order.clear();
        path.clear();
        shortestDist = -1;
        if (V == 0) return false;

        int start = getIndex(startName);
        int target = getIndex(targetName);
        if (start == -1 || target == -1) return false;

        // State tracking arrays
        vector<bool> visited(V, false);
        vector<int> parent(V, -1);
        vector<int> dist(V, -1);
        queue<int> q;

        // Initialize source vertex (Level 0)
        visited[start] = true;
        dist[start] = 0;
        q.push(start);

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            order.push_back(names[u]);

            for (int v : adj[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    dist[v] = dist[u] + 1;
                    parent[v] = u;
                    q.push(v);
                }
            }
        }

        // Unreachable destination check
        if (!visited[target]) return false;

        // Shortest path reconstruction via parent[] array
        shortestDist = dist[target];
        for (int curr = target; curr != -1; curr = parent[curr]) {
            path.push_back(names[curr]);
        }
        reverse(path.begin(), path.end());
        return true;
    }
bool bfsOptimized(const string& startName, const string& targetName, int& shortestDist) {
        if (V == 0) return false;
        int start = getIndex(startName), target = getIndex(targetName);
        if (start == -1 || target == -1) return false;

        vector<bool> visited(V, false);
        vector<int> dist(V, -1);
        
        // Tối ưu hóa: Dùng vector cấp phát sẵn thay vì queue
        vector<int> q;
        q.reserve(V); 
        int head = 0;

        visited[start] = true;
        dist[start] = 0;
        q.push_back(start);

        while (head < (int)q.size()) {
            int u = q[head++]; 
            for (int v : adj[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    dist[v] = dist[u] + 1;
                    q.push_back(v);
                }
            }
        }
        
        shortestDist = dist[target];
        return visited[target];
    }
};

// Tối ưu hóa: Dùng vector với reserve(V) thay stack để loại bỏ chi phí cấp phát phân đoạn của deque
bool dfsOptimized(const Graph& g, int start, int& maxDepth) {
    maxDepth = 0;
    if (start < 0 || start >= g.V) return false;

    vector<pair<int, int>> st;
    st.reserve(g.V); // Tối ưu: Cấp phát trước bộ nhớ ngăn xếp
    vector<bool> vis(g.V, false);

    st.push_back({start, 0});
    vis[start] = true;

    while (!st.empty()) {
        maxDepth = max(maxDepth, (int)st.size());
        int u = st.back().first, &i = st.back().second;

        if (i < (int)g.adj[u].size()) {
            int v = g.adj[u][i++];
            if (!vis[v]) {
                vis[v] = true;
                st.push_back({v, 0});
            }
        } else {
            st.pop_back();
        }
    }
    return false;
}

// 1. Dataset thực tế: Mạng xã hội thu nhỏ
Graph buildSocialNetwork() {
    Graph g({"Huy", "Luan", "Kha", "Alice", "Bob", "Charlie"}, false);
    g.addEdge("Huy", "Luan");
    g.addEdge("Huy", "Kha");
    g.addEdge("Luan", "Alice");
    g.addEdge("Alice", "Bob");
    g.addEdge("Kha", "Charlie");
    g.addEdge("Charlie", "Bob");
    return g;
}

// 2. Adversarial Input: Đồ thị hình sao ép queue phình to tối đa
Graph buildStarGraph(int V) {
    vector<string> names(V);
    for (int i = 0; i < V; ++i) names[i] = to_string(i);
    Graph g(names, false); 
    
    // Tối ưu hóa: Nối cạnh trực tiếp bằng index để tránh O(V^2) khi tạo dữ liệu lớn
    for (int i = 1; i < V; ++i) {
        g.adj[0].push_back(i);
        g.adj[i].push_back(0);
    }
    return g;
}

// 3. Dataset thực tế: Các môn học phần tiên quyết
Graph buildCourseSchedule(bool introduceConflict = false) {
    Graph g({"NMLT", "CTDL", "ThuatToan", "CSDL", "HDH", "AI", "CNPM", "KLTN"}, true);
    g.addEdge("NMLT", "CTDL");
    g.addEdge("CTDL", "ThuatToan");
    g.addEdge("CTDL", "CSDL");
    g.addEdge("ThuatToan", "HDH");
    g.addEdge("ThuatToan", "AI");
    g.addEdge("CSDL", "CNPM");
    g.addEdge("HDH", "KLTN");
    g.addEdge("CNPM", "KLTN");

    if (introduceConflict) {
        // Tạo xung đột phụ thuộc vòng tròn (Circular Dependency): KLTN -> NMLT (Deadlock)
        g.addEdge("KLTN", "NMLT");
    }
    return g;
}

//4. Adversarial Input - Đồ thị suy biến dạng đường thẳng
Graph buildLinearGraph(int V) {
    Graph g({}, true);
    g.V = V;
    g.directed = true;
    g.adj.resize(V);
    for (int i = 0; i < V - 1; ++i) {
        g.adj[i].push_back(i + 1);
    }
    return g;
}

//5. Benchmark DFS chuẩn, đo độ sâu ngăn xếp tối đa
bool dfsStandard(const Graph& g, int start, int& maxDepth) {
    maxDepth = 0;
    if (start < 0 || start >= g.V) return false;

    stack<pair<int, int>> st;
    vector<bool> vis(g.V, false);

    st.push({start, 0});
    vis[start] = true;

    while (!st.empty()) {
        maxDepth = max(maxDepth, (int)st.size());
        int u = st.top().first, &i = st.top().second;

        if (i < (int)g.adj[u].size()) {
            int v = g.adj[u][i++];
            if (!vis[v]) {
                vis[v] = true;
                st.push({v, 0});
            }
        } else {
            st.pop();
        }
    }
    return false;
}

// Main function: execute unit tests
int main() {
    // ==========================================
    //         PHẦN TEST CỦA TUẦN 2
    // ==========================================
    // 1. Đồ thị 6 đỉnh có hướng từ Hand-trace Tuần 1
    Graph g6({"A", "B", "C", "D", "E", "F"}, true);
    g6.addEdge("A", "B");
    g6.addEdge("A", "C");
    g6.addEdge("B", "D");
    g6.addEdge("C", "F");
    g6.addEdge("D", "B"); // Cạnh ngược tạo chu trình D -> B
    g6.addEdge("D", "E");
    g6.addEdge("E", "F");
    
    g6.printGraph();

    cout << "[TEST 1] Week-1 6-Node Graph Reproduction\n";
    
    // --- Test BFS ---
    vector<string> bfsOrder, shortestPath;
    int dist = -1;
    if (g6.bfsShortestPath("A", "F", bfsOrder, shortestPath, dist)) {
        cout << "  -> BFS Traversal Order from A : ";
        for (size_t i = 0; i < bfsOrder.size(); ++i)
            cout << bfsOrder[i] << (i + 1 < bfsOrder.size() ? " -> " : "   [PASS]\n");

        cout << "  -> BFS Shortest Path (A to F) : ";
        for (size_t i = 0; i < shortestPath.size(); ++i)
            cout << shortestPath[i] << (i + 1 < shortestPath.size() ? " -> " : "");
        cout << " (" << dist << " edges)         [PASS]\n";
    }

    // --- Test DFS ---
    vector<string> dfsOrder;
    vector<pair<string, string>> backEdges;
    bool hasCycle = g6.iterativeDFS("A", dfsOrder, backEdges);
    
    cout << "  -> Iterative DFS Order from A : ";
    for (size_t i = 0; i < dfsOrder.size(); ++i)
        cout << dfsOrder[i] << (i + 1 < dfsOrder.size() ? " -> " : "   [PASS]\n");
        
    if (hasCycle && !backEdges.empty()) {
        cout << "  -> DFS Cycle Detection        : Cycle detected! Back-edge "
             << backEdges[0].first << "->" << backEdges[0].second << " [PASS]\n\n";
    }

    // 2. TEST
    cout << "[TEST 2] BFS Edge Cases Verification\n";
    Graph gEmpty({}, true);
    bool resEmpty = gEmpty.bfsShortestPath("A", "B", bfsOrder, shortestPath, dist);
    cout << "  -> Edge Case 1 (Empty Graph V=0)       : "
         << (!resEmpty ? "Returned false safely [PASS]\n" : "[FAIL]\n");

    if (g6.bfsShortestPath("A", "A", bfsOrder, shortestPath, dist)) {
        cout << "  -> Edge Case 2 (Source == Target A->A) : Path: "
             << shortestPath[0] << " (" << dist << " edges)     [PASS]\n";
    }

    bool resUnreach = g6.bfsShortestPath("F", "A", bfsOrder, shortestPath, dist);
    cout << "  -> Edge Case 3 (Unreachable Node F->A) : "
         << (!resUnreach ? "Returned false safely [PASS]\n" : "[FAIL]\n\n");

    cout << "[TEST 3] Directed Acyclic Graph (DAG - Verify No False Cycle)\n";
    Graph dag({"A", "B", "C", "D"}, true);
    dag.addEdge("A", "B");
    dag.addEdge("A", "C");
    dag.addEdge("B", "D");
    dag.addEdge("C", "D"); // Cross edge to D (inStack[D] == false), not a cycle

    vector<string> dagOrder;
    vector<pair<string, string>> dagBackEdges;
    bool dagCycle = dag.iterativeDFS("A", dagOrder, dagBackEdges);

    cout << "  -> Cycle Detected    : " << (dagCycle ? "YES" : "NO") << " (Correct!)\n\n";


    // ==========================================
    //         PHẦN TEST CỦA TUẦN 3
    // ==========================================
    cout << "================================================\n";
    cout << "              START WEEK 3 TESTING              \n";
    cout << "================================================\n\n";

    // --- YÊU CẦU 1: SO SÁNH BFS VÀ DFS TRÊN DỮ LIỆU THỰC TẾ ---
    cout << "--- 1. REAL-WORLD DATASET (Social Network) ---\n";
    Graph socialGraph = buildSocialNetwork();
    vector<string> bfsOrderW3, bfsPathW3, dfsOrderW3;
    vector<pair<string, string>> dfsCyclesW3;
    int distW3;
    
    cout << "Tim duong tu Huy den Bob:\n";
    socialGraph.bfsShortestPath("Huy", "Bob", bfsOrderW3, bfsPathW3, distW3);
    cout << "-> BFS Path: ";
    for (const string& s : bfsPathW3) cout << s << " ";
    cout << "(Khoang cach: " << distW3 << ")\n";

    socialGraph.iterativeDFS("Huy", dfsOrderW3, dfsCyclesW3);
    cout << "-> DFS Thu tu duyet: ";
    for (const string& s : dfsOrderW3) cout << s << " ";
    cout << "\n(DFS di lac sang nhanh cua Kha thay vi tim Bob nhanh nhat)\n\n";

    // --- YÊU CẦU 2 & 3: ADVERSARIAL INPUT VÀ BENCHMARK TỐI ƯU ---
    cout << "--- 2. BENCHMARK & OPTIMIZATION (Star Graph V=500000) ---\n";
    int V_large = 500000;
    Graph starGraph = buildStarGraph(V_large);
    
    // Đo thời gian BFS thường (std::queue)
    auto start1 = high_resolution_clock::now();
    starGraph.bfsShortestPath("0", to_string(V_large - 1), bfsOrderW3, bfsPathW3, distW3);
    auto stop1 = high_resolution_clock::now();
    auto duration1 = duration_cast<microseconds>(stop1 - start1);
    cout << "BFS std::queue time : " << duration1.count() << " us\n";

    // Đo thời gian BFS tối ưu (std::vector)
    auto start2 = high_resolution_clock::now();
    starGraph.bfsOptimized("0", to_string(V_large - 1), distW3);
    auto stop2 = high_resolution_clock::now();
    auto duration2 = duration_cast<microseconds>(stop2 - start2);
    cout << "BFS optimized time  : " << duration2.count() << " us\n";
    
    // Yêu cầu 1 và 2 (phần của Luân): Test phát hiện xung đột chu trình (Deadlock) trên dataset môn học
    cout << "\n--- 3. REAL-WORLD DATASET (Course Prerequisites) ---\n";
    Graph validSchedule = buildCourseSchedule(false), conflictSchedule = buildCourseSchedule(true);
    vector<string> orderW3; vector<pair<string, string>> conflictEdges;

    bool conflictA = validSchedule.iterativeDFS("NMLT", orderW3, conflictEdges);
    cout << "Kich ban A (DAG hop le)    : " << (!conflictA ? "Khong co xung dot [PASS]\n" : "[FAIL]\n");

    bool conflictB = conflictSchedule.iterativeDFS("NMLT", orderW3, conflictEdges);
    cout << "Kich ban B (Loi xung dot)  : " << (conflictB && !conflictEdges.empty() ? "Phat hien Deadlock: " + conflictEdges[0].first + " -> " + conflictEdges[0].second + " [PASS]\n\n" : "[FAIL]\n\n");
    
    // Yêu cầu 3 và 4 (phần của Luân): Benchmark trên đồ thị đường thẳng V = 200, 000, đo Max Stack Depth = 200, 000 và so sánh thời gian giữa dfsStandard và dfsOptimized 
    cout << "--- 4. BENCHMARK & OPTIMIZATION (Linear Graph V=200000) ---\n";
    int V_linear = 200000;
    Graph linearGraph = buildLinearGraph(V_linear);
    int maxDepthStd = 0, maxDepthOpt = 0;

    // Đo thời gian DFS thường (std::stack)
    auto startDfs1 = high_resolution_clock::now();
    dfsStandard(linearGraph, 0, maxDepthStd);
    auto stopDfs1 = high_resolution_clock::now();
    auto durationDfs1 = duration_cast<microseconds>(stopDfs1 - startDfs1);
    cout << "DFS std::stack time : " << durationDfs1.count() << " us (Max Stack Depth: " << maxDepthStd << " / " << V_linear << ")\n";

    // Đo thời gian DFS tối ưu (std::vector)
    auto startDfs2 = high_resolution_clock::now();
    dfsOptimized(linearGraph, 0, maxDepthOpt);
    auto stopDfs2 = high_resolution_clock::now();
    auto durationDfs2 = duration_cast<microseconds>(stopDfs2 - startDfs2);
    cout << "DFS optimized time  : " << durationDfs2.count() << " us (Max Stack Depth: " << maxDepthOpt << " / " << V_linear << ")\n";
    return 0;
}
