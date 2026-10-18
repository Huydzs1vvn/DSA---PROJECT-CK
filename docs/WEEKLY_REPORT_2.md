
---

# WEEK 2 REPORT: CORE IMPLEMENTATION

## PART 1: GROUP PROGRESS SUMMARY

### 1. Current Status & Core Implementation
In Week 2, our group successfully implemented the core graph data structures and traversal algorithms required for the project, directly addressing the rubric's "Core algorithm/structure running" requirement:
- **Graph Representation:** Built a flexible `Graph` class using an Adjacency List (`vector<vector<int>>`) that supports both directed and undirected graphs.
- **Iterative DFS & Cycle Detection:** Implemented Depth-First Search using an explicit `std::stack` (strictly avoiding recursion) with a 3-color state tracking mechanism (`visited` and `inStack`) to correctly detect cycles via back-edges.
- **BFS & Shortest Path:** Implemented Breadth-First Search using `std::queue` to compute the shortest path in unweighted graphs and reconstruct the exact route using a `parent[]` array.
- **Testing & Verification:** Authored comprehensive Unit Tests that perfectly reproduce our 6-node graph hand-traces from Week 1. We also explicitly tested edge cases (empty graph, unreachable target, source equals target).

### 2. Task Allocation (Week 2)

| Member | Assigned Tasks | Completion |
| :--- | :--- | :--- |
| **Vũ Thành Huy** | - Implement BFS algorithm using `std::queue` and shortest path reconstruction via `parent[]` array.<br>- Write Unit Tests reproducing Week-1 BFS hand-trace and capture verifiable test output.<br>- Draft time & space complexity derivation $O(V + E)$ for BFS based on code loop structures.<br>- Identify, verify, and document AI errors regarding BFS edge cases (empty graph, unreachable node).<br>- Author Teach-back (line-level explanation, queue invariant) & compile Group Progress/Syllabus Mapping. | 100% |
| **Lê Thiện Luân** | - Construct core `Graph` class using Adjacency List supporting directed/undirected graphs.<br>- Implement Iterative DFS using an explicit `std::stack` and Cycle Detection via active path tracking.<br>- Write Unit Tests reproducing Week-1 DFS hand-trace, verify back-edges, and capture test output.<br>- Draft time & space complexity derivation $O(V + E)$ for Iterative DFS.<br>- Identify, verify, and document AI logic errors regarding state backtracking in Iterative DFS.<br>- Author individual Teach-back (line-level explanation, stack loop invariant). | 100% |

### 3. Syllabus Alignment Mapping

| Syllabus Topic (Appendix A - Project #19) | Course Concept Exercised | Implementation Location in Repository |
| :--- | :--- | :--- |
| **Graphs (Representation)** | Adjacency List (`vector<vector<int>>`), Directed/Undirected switch, $O(V + E)$ space | `graph.cpp` (Lines 9–54: `struct Graph`, `addEdge`, `getIndex`) |
| **Stacks (Lesson 8 - L8)** | Explicit `std::stack<pair<int, int>>` simulating recursion frames, LIFO backtracking, cycle detection (`visited` + `inStack`) | `graph.cpp` (Lines 55–110: `Graph::iterativeDFS`) |
| **Queues (Lesson 8 - L8)** | FIFO `std::queue<int>` frontier expansion, Two-Layer distance property, `parent[]` shortest-path tree reconstruction | `graph.cpp` (Lines 112–159: `Graph::bfsShortestPath`) |
| **Complexity Analysis (Lesson 2 - L2)** | Derivation of $T(V, E) = O(V + E)$ and $S(V, E) = O(V + E)$ from actual loop structures and Handshaking Lemma | `docs/WEEKLY_REPORT_2.md` (Part 2 & 3 of both members) |

---

# Academic Report: Time & Space Complexity Proof and Teach-Back on Iterative DFS (LE THIEN LUAN)

## PART 1: MATHEMATICAL PROOF OF TIME AND SPACE COMPLEXITY O(V + E)

### 1. Mathematical Model and Notation
Let $G = (V, E)$ be a graph with vertex set $V$ and edge set $E$.
- For each vertex $u \in V$, let $\text{adj}[u]$ denote the adjacency list containing all neighbors adjacent from $u$.
- Out-degree of $u$: $\text{deg}^+(u) = |\text{adj}[u]|$.
- In-degree of $u$: $\text{deg}^-(u)$.
- **Handshaking Lemma:**
  - For directed graphs: $\sum_{u \in V} \text{deg}^+(u) = E$.
  - For undirected graphs: $\sum_{u \in V} \text{deg}(u) = 2E$.

---

### 2. Time Complexity Proof: T(V, E) = O(V + E)

The total running time of `iterativeDFS` consists of three fundamental components:

$$
T(V, E) = T_{\text{init}} + T_{\text{stack}} + T_{\text{edges}}
$$

#### (1) Initialization Cost ($T_{\text{init}}$)
- Allocating and initializing boolean arrays:
  - `visited`: Vector of size $V$, filled with `false` $\to \Theta(V)$.
  - `inStack`: Vector of size $V$, filled with `false` $\to \Theta(V)$.
  - `parent`: Vector of size $V$, filled with `-1` $\to \Theta(V)$.
- Initializing the root vertex `start`:
  `visited[start] = true; inStack[start] = true; st.push({start, 0});` takes $O(1)$ time.

$$
T_{\text{init}} = \Theta(V)
$$

#### (2) Stack Operations Cost ($T_{\text{stack}}$)
- **Push Operations:**
  - A vertex $v$ is pushed onto the stack `st` exclusively within the condition block `if (!visited[v])` (Lines 86–93).
  - Immediately upon being pushed, `visited[v]` is set to `true` (Line 89).
  - Because elements in `visited` only transition monotonically from `false` to `true` and are **never reset to `false`**, each vertex $v \in V$ satisfies `!visited[v]` and is pushed onto the stack **at most once**.
- **Pop Operations:**
  - A vertex $u$ is popped from `st` if and only if all of its adjacent edges have been fully inspected, i.e., $i == |\text{adj}[u]|$ (Lines 102–106).
  - Once popped, a vertex can never be pushed again since `visited[u] == true`. Hence, each vertex $u \in V$ is popped from the stack **at most once**.
- Each individual operation (`push`, `pop`, `top`, `empty`) on `std::stack` executes in $O(1)$ amortized time.

$$
T_{\text{stack}} = \sum_{u \in V} \left( O(1)_{\text{push}} + O(1)_{\text{pop}} \right) = O(V)
$$

#### (3) Edge Examination Cost ($T_{\text{edges}}$)
- For every vertex $u$ on top of the stack, the neighbor index $i$ (stored in `st.top().second`) monotonically increases from $0$ up to $|\text{adj}[u]|$:
  ```cpp
  int v = adj[u][i++];
  ```
- By declaring `int& i = st.top().second;` as a reference (Line 77), the iteration state is preserved during backtracking. This guarantees that each outgoing edge in `adj[u]` is examined exactly once.
- For each edge $(u, v)$, the algorithm performs:
  - Undirected trivial edge check: `!directed && v == parent[u]` $\to O(1)$ (Line 84).
  - Array lookups and conditional checks: `!visited[v]` and `inStack[v]` $\to O(1)$ (Lines 86, 94).
  - Recording cycle edge: `backEdges.push_back(...)` $\to O(1)$ (Line 97).
- Summing across all vertices:

$$
T_{\text{edges}} = O(E)
$$

#### Total Time Complexity Conclusion:

$$
T(V, E) = T_{\text{init}} + T_{\text{stack}} + T_{\text{edges}} = O(V) + O(V) + O(E) = O(V + E)
$$

---

### 3. Space Complexity Proof: S(V, E) = O(V + E)

The total auxiliary memory consumed by the algorithm is defined by:

$$
S(V, E) = S_{\text{graph}} + S_{\text{stack}} + S_{\text{arrays}}
$$

1. **Graph Representation Memory ($S_{\text{graph}}$):**
   - The adjacency list `vector<vector<int>> adj` consists of an outer vector of size $V$, with the total number of stored integers equal to $E$ (for directed) or $2E$ (for undirected).
   $$S_{\text{graph}} = O(V + E)$$
2. **Explicit Call Stack Memory ($S_{\text{stack}}$):**
   - The explicit stack `stack<pair<int, int>> st` stores the vertices along the current active search path.
   - Best-case scenario (Star Graph): The stack depth never exceeds $O(1)$.
   - Worst-case scenario (Linear Degenerate Graph $1 \to 2 \to 3 \to \dots \to V$): All $V$ vertices reside simultaneously in the stack.
   $$S_{\text{stack}} \le V \times O(1) = O(V)$$
3. **State Tracking Vectors ($S_{\text{arrays}}$):**
   - `visited`: Vector of $V$ booleans $\to O(V)$.
   - `inStack`: Vector of $V$ booleans $\to O(V)$.
   - `parent`: Vector of $V$ integers $\to O(V)$.
   - `order`: Output array storing at most $V$ string labels $\to O(V)$.
   $$S_{\text{arrays}} = O(V)$$

#### Total Space Complexity Conclusion:

$$
S(V, E) = O(V + E) + O(V) + O(V) = O(V + E)
$$

---

## PART 2: INDIVIDUAL TEACH-BACK & CODE SELF-EXPLANATION 

### 1. Line-level Code Explanation

The method `Graph::iterativeDFS` eliminates compiler recursion entirely, simulating the call stack explicitly:

```cpp
55: bool iterativeDFS(const string& startName, vector<string>& order, vector<pair<string, string>>& backEdges) {
56:     int start = getIndex(startName);
57:     if (start == -1) return false;
```
- **Lines 55–57:** Function entry taking the starting vertex name (`startName`), output discovery sequence (`order`), and detected cycle edges (`backEdges`). If the starting vertex does not exist, the function terminates safely by returning `false`.

```cpp
59:     // visited[u]: Whether vertex u has been discovered
60:     vector<bool> visited(V, false);
61:     // inStack[u]: Whether vertex u is on the active search path in stack
62:     vector<bool> inStack(V, false);
63:     vector<int> parent(V, -1);
64:     bool hasCycle = false;
```
- **Lines 59–64:** Allocates state structures:
  - `visited[u]`: Tracks whether vertex $u$ has ever been discovered (prevents redundant processing).
  - `inStack[u]`: Tracks whether vertex $u$ is currently on the active search path in the stack (identifies back-edges).
  - `parent[u]`: Records the immediate predecessor of $u$ in the DFS tree.
  - `hasCycle`: Boolean flag indicating cycle presence.

```cpp
66:     // Stack storing frames: (current vertex u, next edge index to inspect)
67:     stack<pair<int, int>> st;
```
- **Lines 66–67:** Defines the explicit stack. Each element is a `pair<int, int>` representing a **Stack Frame**:
  - `first` (int $u$): The vertex currently being explored.
  - `second` (int $i$): The index of the next adjacent edge to inspect in `adj[u]`.

```cpp
69:     // Initialize root vertex
70:     visited[start] = true;
71:     inStack[start] = true;
72:     order.push_back(names[start]);
73:     st.push({start, 0});
```
- **Lines 69–73:** Root Initialization (Root Push):
  - Marks `start` as discovered (`visited = true`) and present in stack (`inStack = true`).
  - Appends `start` to the discovery order.
  - Pushes frame `{start, 0}` to start scanning from neighbor index $0$.

```cpp
75:     while (!st.empty()) {
76:         int u = st.top().first;
77:         int& i = st.top().second; // Reference to track edge index across backtracking
```
- **Lines 75–77:** Main iteration loop:
  - Inspects the top frame without popping: `u = st.top().first`.
  - Captures `int& i = st.top().second;` as a **reference (`&`)**. When `i++` executes, the updated index is stored directly in the stack element. Upon returning from deeper subtrees, exploration resumes at the subsequent neighbor without restarting from zero.

```cpp
79:         // If vertex u still has unexplored adjacent edges
80:         if (i < (int)adj[u].size()) {
81:             int v = adj[u][i++]; // Fetch neighbor and advance index
```
- **Lines 79–81:** Checks if vertex $u$ has remaining unexplored neighbors. If yes, extracts neighbor $v = \text{adj}[u][i]$ and increments pointer $i$.

```cpp
83:             // In undirected graphs, ignore trivial back-edge to direct parent
84:             if (!directed && v == parent[u]) continue;
```
- **Lines 83–84:** Handles undirected graphs: The edge $(u, \text{parent}[u])$ is a trivial bidirectional edge, not a cycle. Skipping this prevents false-positive cycle detection.

```cpp
86:             if (!visited[v]) {
87:                 // Tree Edge: vertex v is undiscovered -> push to stack, mark inStack
88:                 parent[v] = u;
89:                 visited[v] = true;
90:                 inStack[v] = true;
91:                 order.push_back(names[v]);
92:                 st.push({v, 0});
93:             }
```
- **Lines 86–93 (Forward Exploration / Tree Edge):**
  - If neighbor $v$ is undiscovered (`!visited[v]`): Edge $(u, v)$ is a **Tree Edge**.
  - Sets `parent[v] = u`, marks `visited[v] = true` and `inStack[v] = true`.
  - Pushes new frame `{v, 0}` onto the stack to explore $v$'s descendants immediately in the next iteration.

```cpp
94:             else if (inStack[v]) {
95:                 // Back-Edge: vertex v is currently in stack (ancestor) -> CYCLE DETECTED!
96:                 hasCycle = true;
97:                 backEdges.push_back({names[u], names[v]});
98:             }
```
- **Lines 94–98 (Cycle Detection / Back-edge):**
  - If $v$ is already visited and `inStack[v] == true`: Vertex $v$ is an active ancestor currently residing on the stack.
  - Therefore, edge $(u, v)$ is a **Back-edge** that closes a directed loop.
  - Sets `hasCycle = true` and records edge $(u, v)$.
  - *(Note: If `visited[v] == true` but `inStack[v] == false`, vertex $v$ belongs to an already completed subtree — a cross or forward edge — which is safely ignored).*

```cpp
102:        else {
103:            // All neighbors of u explored -> Pop from stack (Backtrack)
104:            inStack[u] = false;
105:            st.pop();
106:        }
```
- **Lines 102–106 (Backtracking):**
  - When $i == |\text{adj}[u]|$, all neighbors and subtrees of $u$ have been fully explored.
  - Clears `inStack[u] = false` (vertex $u$ exits the active path).
  - Pops $u$ from the stack (`st.pop()`), returning control to $u$'s parent.

```cpp
109:    return hasCycle;
```
- **Line 109:** Returns the overall cycle detection result.

---

### 2. Loop Invariant Analysis for the Explicit Stack

#### a. Loop Invariant Statement (Concise & Intuitive)
At the start of each iteration of the loop `while (!st.empty())` (Line 75), the following three conditions **always hold simultaneously**:

1. **Active Stack Path:** The vertices stored from the bottom to the top of the stack always form a **single direct path (active search branch)** from source vertex $s$ to the current vertex $u$:
   $$s \to \dots \to \text{parent}[u] \to u$$
2. **Meaning of the `inStack` Array:**
   - `inStack[x] == true`: Vertex $x$ is **currently in the stack** (meaning $x$ is an active ancestor of current vertex $u$).
   - `visited[x] == true` and `inStack[x] == false`: Vertex $x$ and its entire subtree have been **completely explored** and popped.
   - `visited[x] == false`: Vertex $x$ has **never been discovered**.
3. **Sound Cycle Detection Basis:** If the current vertex $u$ has an adjacent edge to vertex $v$ where `inStack[v] == true`, then $v$ is definitively an active ancestor on the stack $\to$ edge $(u, v)$ points backward to an ancestor, strictly confirming a **cycle**.

---

#### b. Correctness Proof (3 Concise Steps)

1. **Initialization (Before the loop begins):**
   - The stack contains only the source vertex: $[s]$, with `inStack[s] = true`.
   - The path consists solely of $\{s\}$ (valid). No vertex is completed yet. $\to$ Invariant holds.

2. **Maintenance (During each iteration):**
   - **Forward Exploration (Tree Edge - Lines 86–93):** When visiting undiscovered vertex $v$ (`!visited[v]`), we push $v$ onto the stack and set `inStack[v] = true`. The path extends by edge $u \to v$. $\to$ Invariant is preserved.
   - **Cycle Detection (Back-Edge - Lines 94–98):** When encountering $v$ with `inStack[v] == true` (ancestor in stack), edge $(u, v)$ closes a cycle. The algorithm asserts `hasCycle = true`. The stack is unchanged. $\to$ Invariant is preserved.
   - **Skip Completed Subtrees:** If `visited[v] == true` and `inStack[v] == false` (cross/forward edge), $v$'s branch was already fully processed. It is safely ignored without modifying the stack. $\to$ Invariant is preserved.
   - **Backtracking (Lines 102–106):** When all outgoing edges of $u$ are finished, the algorithm sets `inStack[u] = false` and pops $u$. The stack shrinks back to the path ending at $u$'s parent. $\to$ Invariant is preserved.

3. **Termination (When the stack is empty):**
   - All vertices and edges reachable from source $s$ have been completely processed (`inStack = false`).
   - Every cycle (if any existed) was correctly flagged via back-edges pointing to `inStack == true` vertices. The algorithm terminates with 100% correctness.


---


# Academic Report: Time & Space Complexity Proof and Teach-Back on BFS & Shortest Path (VU THANH HUY)

## PART 1: MATHEMATICAL PROOF OF TIME AND SPACE COMPLEXITY O(V + E)

### 1. Mathematical Model and Notation
Synchronized with our core `Graph` representation, let $G = (V, E)$ be an unweighted graph (directed or undirected) with vertex set $V$ and edge set $E$:
- For each vertex $u \in V$, let $\text{adj}[u]$ denote its adjacency list containing all outgoing neighbors $v$.
- Out-degree of vertex $u$: $\text{deg}^+(u) = \vert{}\text{adj}[u]\vert{}$.
- Shortest-path distance (in edge count) from source $s$ to vertex $u$ is denoted as $\delta(s, u)$.
- **Handshaking Lemma:**
  - For directed graphs: $\sum_{u \in V} \text{deg}^+(u) = E$.
  - For undirected graphs: $\sum_{u \in V} \text{deg}(u) = 2E$.

---

### 2. Time Complexity Proof: T(V, E) = O(V + E)

The total execution time of `bfsShortestPath` is decomposed into four distinct phases based on the actual loop structures in the code:

$$T(V, E) = T_{\text{init}} + T_{\text{queue}} + T_{\text{edges}} + T_{\text{path}}$$

#### (1) Initialization Cost ($T_{\text{init}}$)
- Vertex index lookups via `getIndex(startName)` and `getIndex(targetName)`: Since `getIndex` performs a linear scan over the `names` vector of size $V$, each lookup strictly takes $O(V)$ time.
- Allocating and initializing tracking arrays of size $V$:
  - `visited`: Vector of size $V$, initialized to `false` $\to \Theta(V)$.
  - `parent`: Vector of size $V$, initialized to `-1` $\to \Theta(V)$.
  - `dist`: Vector of size $V$, initialized to `-1` $\to \Theta(V)$.
- Initializing the source vertex `start` (`visited`, `dist`, `q.push`) takes $O(1)$ time.

$$T_{\text{init}} = O(V) + \Theta(V) + O(1) = \Theta(V)$$

#### (2) Queue Operations Cost ($T_{\text{queue}}$)
- **Enqueue (`push`) Operations:**
  - A neighbor $v$ is pushed into the FIFO queue `q` exclusively inside the conditional block `if (!visited[v])` (Lines 140–145).
  - Immediately upon entering this block, `visited[v]` is set to `true` (Line 141).
  - Because `visited[v]` transitions monotonically from `false` to `true` and is **never reset**, each vertex $v \in V$ satisfies `!visited[v]` at most once and is enqueued **at most once**.
- **Dequeue (`pop`) Operations:**
  - In each iteration of `while (!q.empty())` (Line 134), exactly one vertex $u$ is extracted via `q.front()` and removed via `q.pop()` (Lines 135–136).
  - Since each vertex is enqueued at most once, each vertex $u \in V$ is dequeued **at most once**.
- Queue operations (`push`, `pop`, `front`, `empty`) on `std::queue` run in $O(1)$ time.

$$T_{\text{queue}} = \sum_{u \in V} \left( O(1)_{\text{push}} + O(1)_{\text{pop}} \right) = O(V)$$

#### (3) Edge Examination Cost ($T_{\text{edges}}$)
- When a vertex $u$ is dequeued, the inner loop `for (int v : adj[u])` (Line 139) iterates through all adjacent vertices in $\text{adj}[u]$, performing exactly $\text{deg}^+(u)$ iterations.
- Inside the inner loop, checking `!visited[v]` and updating states requires $O(1)$ constant time per edge.
- By the Handshaking Lemma, summing over all dequeued vertices $u \in V$:

$$T_{\text{edges}} = \sum_{u \in V} \text{deg}^+(u) \times O(1) = O(E)$$

#### (4) Shortest Path Reconstruction Cost ($T_{\text{path}}$)
- After traversal, the backtracking loop `for (int curr = target; curr != -1; curr = parent[curr])` (Lines 154–156) traces predecessors from `target` back to `start`.
- The shortest path length is bounded by $\delta(\text{start}, \text{target}) \le V - 1$.
- Reversing the `path` vector (Line 157) takes time linear in path length:

$$T_{\text{path}} = O(V)$$

#### Total Time Complexity Conclusion:
$$T(V, E) = \Theta(V) + O(V) + O(E) + O(V) = O(V + E)$$

---

### 3. Space Complexity Proof: S(V, E) = O(V + E)

The total auxiliary memory footprint is governed by:

$$S(V, E) = S_{\text{queue}} + S_{\text{arrays}}$$

1. **FIFO Queue Memory ($S_{\text{queue}}$):**
   - `std::queue<int> q` holds discovered frontier vertices.
   - Worst-case scenario (Star Graph): All $V - 1$ neighbors are enqueued simultaneously on level 1.
   $$S_{\text{queue}} \le V \times O(1) = O(V)$$
2. **Auxiliary Tracking Vectors ($S_{\text{arrays}}$):**
   - `visited`, `parent`, and `dist`: Vectors of size $V \to 3 \times O(V) = O(V)$.
   - `order` and `path`: Output vectors holding at most $V$ elements $\to O(V)$.
   $$S_{\text{arrays}} = O(V)$$

*Note: The shared adjacency list $S_{\text{graph}} = O(V + E)$ is part of the class structure, but considering traversal overhead alone:*
#### Total Space Complexity Conclusion:
$$S(V, E) = O(V) + O(V) = O(V) \text{ auxiliary } \implies O(V + E) \text{ inclusive of graph}.$$

---

## PART 2: INDIVIDUAL TEACH-BACK & CODE SELF-EXPLANATION

### 1. Line-level Code Explanation

The method `Graph::bfsShortestPath` executes breadth-first traversal using `std::queue` and reconstructs the unweighted shortest path via the `parent[]` predecessor tree:

```cpp
112: bool bfsShortestPath(const string& startName, const string& targetName,
113:                      vector<string>& order, vector<string>& path, int& shortestDist) {
114:     order.clear();
115:     path.clear();
116:     shortestDist = -1;
117:     if (V == 0) return false;
```
Lines 112–117: Function signature accepting source, destination, and output containers. Clears output vectors and handles the empty graph ($V = 0$) edge case immediately to prevent out-of-bounds errors on state arrays.

```cpp
119:     int start = getIndex(startName);
120:     int target = getIndex(targetName);
121:     if (start == -1 || target == -1) return false;
```
Lines 119–121: Maps string vertex names to internal integer indices. If either the source or target vertex does not exist, the function terminates safely by returning false.

```cpp
123:     // State tracking arrays
124:     vector<bool> visited(V, false);
125:     vector<int> parent(V, -1);
126:     vector<int> dist(V, -1);
127:     queue<int> q;
```

Lines 123–127: Allocates BFS state structures:
- visited: Prevents re-enqueueing already discovered vertices (avoids infinite loops).
- parent: Stores the immediate predecessor vertex that first discovered $u$.
- dist: Stores the minimum edge count from start to $u$.
- q: Standard FIFO queue managing the breadth-first frontier layer by layer.

```cpp
129:     // Initialize source vertex (Level 0)
130:     visited[start] = true;
131:     dist[start] = 0;
```

Lines 129–132: Root Initialization (Level 0). Marks start as visited at distance $0$ and pushes it into the queue to initiate expansion.

```cpp
134:     while (!q.empty()) {
135:         int u = q.front();
136:         q.pop();
137:         order.push_back(names[u]);
```

Lines 134–137: Extracts the oldest vertex $u$ from the front of the queue (q.front()) and removes it (q.pop()). Appends names[u] to order to record the exact visitation sequence.

```cpp
139:         for (int v : adj[u]) {
140:             if (!visited[v]) {
141:                 visited[v] = true;
142:                 dist[v] = dist[u] + 1;
143:                 parent[v] = u;
144:                 q.push(v);
145:             }
146:         }
147:     }
```

Lines 139–147 (Frontier Expansion):
- Iterates over every outgoing neighbor $v \in \text{adj}[u]$.
- If $v$ is unvisited (!visited[v]):
+ Line 141: Marks visited[v] = true immediately before pushing to prevent duplicate queue entries.
+ Lines 142-143: Sets level distance dist[v] = dist[u] + 1 and records the shortest path tree edge parent[v] = u.
+ Line 144: Enqueues $v$ to the back of q for next-layer processing.

```cpp
149:     // Unreachable destination check
150:     if (!visited[target]) return false;
```

Lines 149–150: Critical Edge-Case Check. If target was never reached during BFS (!visited[target]), there is no valid path. Returns false to avoid infinite loops during backtracking.

```cpp
152:     // Shortest path reconstruction via parent[] array
153:     shortestDist = dist[target];
154:     for (int curr = target; curr != -1; curr = parent[curr]) {
155:         path.push_back(names[curr]);
156:     }
157:     reverse(path.begin(), path.end());
158:     return true;
159: }
```

Lines 152–159 (Backtracking):
- Traces backward from target to start by following curr = parent[curr] until hitting the root sentinel -1.
- Reverses path so the sequence reads forward from startName to targetName and returns true.

### 2. Loop Invariant Analysis for the FIFO Queue (`std::queue`)

#### a. Loop Invariant Statement (Concise & Intuitive)
At the start of each iteration of the loop `while (!q.empty())` (Line 134), the following three conditions **always hold simultaneously**:

1. **Queue Monotonicity (Two-Layer Property):** If the queue $q = [v_1, v_2, \dots, v_k]$ is non-empty, the distances of vertices from front to back are monotonically non-decreasing and span **at most two consecutive distance layers**:

$$\text{dist}[v_1] \le \text{dist}[v_2] \le \dots \le \text{dist}[v_k] \le \text{dist}[v_1] + 1$$

2. **Exact Shortest-Path Optimality of Visited Vertices:** Every vertex $x \in V$ with `visited[x] == true` has been assigned its true shortest-path edge count $\text{dist}[x] = \delta(\text{start}, x)$, and all vertices with $\delta(\text{start}, y) < \text{dist}[v_1]$ have already been completely dequeued and expanded.

3. **Valid Predecessor Tree (`parent[]`):** For every discovered vertex $x \neq \text{start}$ (`visited[x] == true`), `parent[x]` points to a vertex $p$ such that $(p, x) \in E$ and $\text{dist}[x] = \text{dist}[p] + 1$, forming a valid rooted shortest-path tree from `start`.

#### b. Correctness Proof (3 Concise Steps)

1. **Initialization (Before the first iteration):**
   - Queue contains only $q = [\text{start}]$ with $\text{dist}[\text{start}] = 0 = \delta(\text{start}, \text{start})$, `visited[start] = true`, and `parent[start] = -1`.
   - Two-layer property holds trivially ($0 \le 0 + 1$), and the predecessor tree is valid $\to$ Invariant holds.

2. **Maintenance (During each iteration):**
   - Let $u = v_1$ be dequeued from the front of $q$ with distance $d = \text{dist}[u]$. By the invariant, any remaining vertices in $q$ have distance $d$ or $d + 1$.
   - For each unvisited neighbor $v \in \text{adj}[u]$ (`!visited[v]`):
     - Since all vertices at distance $< d$ were already expanded, no shorter path to $v$ can exist; thus $\delta(\text{start}, v) = d + 1$.
     - Setting `dist[v] = d + 1` and pushing $v$ to the back of $q$ appends elements of distance $d + 1$ behind existing elements of distance $d$ or $d + 1$, preserving the **Two-Layer Property**.
     - Setting `parent[v] = u` links $v$ to $u$ where $\text{dist}[v] = \text{dist}[u] + 1$, preserving the **Predecessor Tree**.
   - Already-visited neighbors (`visited[v] == true`) already achieved distance $\le d + 1$ earlier and are skipped $\to$ Invariant is preserved.

3. **Termination (When the queue is empty):**
   - `q.empty() == true` implies every vertex $x$ reachable from `start` has been visited with $\text{dist}[x] = \delta(\text{start}, x)$ and linked via `parent[x]`.
   - Tracing `parent[]` from `target` back to `start` is guaranteed to terminate in $\delta(\text{start}, \text{target})$ steps and yield a minimum-edge path.

---

## 3. Unit Test Execution & AI Edge-Case Verification Summary

### a. Reproducing Week-1 6-Node Hand Trace (Unit Test Output)
Running our automated test suite (`tests/test_traversal.cpp`) on the exact 6-node directed graph from Week 1 ($V = \{A, B, C, D, E, F\}$, $E = \{(A,B), (A,C), (B,D), (C,F), (D,B), (D,E), (E,F)\}$) produces:

```text
[TEST 1] Week-1 6-Node Graph Reproduction
  -> BFS Traversal Order from A : A -> B -> C -> D -> F -> E   [PASS]
  -> BFS Shortest Path (A to F) : A -> C -> F (2 edges)         [PASS]
  -> Iterative DFS Order from A : A -> B -> D -> E -> F -> C   [PASS]
  -> DFS Cycle Detection        : Cycle detected! Back-edge D->B [PASS]

[TEST 2] BFS Edge Cases Verification
  -> Edge Case 1 (Empty Graph V=0)       : Returned false safely [PASS]
  -> Edge Case 2 (Source == Target A->A) : Path: A (0 edges)     [PASS]
  -> Edge Case 3 (Unreachable Node F->A) : Returned false safely [PASS]
```

### b. Critical Verification of AI Output (BFS Edge Cases)

- **Identified AI Flaw:** The initial AI-generated BFS reconstruction traced `while (curr != start)` and unconditionally pushed `start` at the end without checking `if (!visited[target])`.
- **How We Found Out:** When writing Unit Test 2 (Edge Case 3: unreachable target $F \to A$ where $F$ has out-degree 0), the AI code failed to detect unreachability and falsely returned path `A -> F` with distance 1. Furthermore, when `startName` was not in the graph (`getIndex == -1`), it caused an out-of-bounds vector crash (`visited[-1]`).
- **Our Fix:** Added guard clauses `if (V == 0)` and `if (start == -1 || target == -1)`, enforced `if (!visited[target]) return false;` before backtracking, and unified the backtracking loop condition to `for (int curr = target; curr != -1; curr = parent[curr])`.

---
