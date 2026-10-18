| Member | Assigned Tasks |
| :--- | :--- |
| **Vũ Thành Huy** | <ul><li>Implement BFS algorithm using `std::queue` and implement Shortest Path reconstruction using the `parent[]` array.</li><li>Write Unit Tests replicating the exact Week-1 BFS hand-trace on the 6-node graph and export execution test output evidence.</li><li>Draft the mathematical proof for BFS time and space complexity $O(V + E)$ based on the code's loop structure.</li><li>Identify, verify, and document AI failures/limitations when handling BFS edge cases (empty graph, unreachable destination, source identical to destination).</li><li>Author individual Teach-back section: provide a line-level code explanation, state the loop invariant for the queue, and compile the group report (including Syllabus Mapping).</li></ul> |
| **Lê Thiện Luân** | <ul><li>Construct the core `Graph` class using an Adjacency List representation supporting both directed and undirected graphs.</li><li>Implement the Iterative DFS algorithm using an explicit `std::stack` (strictly avoiding recursion) and integrate Cycle Detection.</li><li>Write Unit Tests replicating the Week-1 DFS hand-trace, verify back-edges, and export execution test output evidence.</li><li>Draft the mathematical proof for Iterative DFS time and space complexity $O(V + E)$.</li><li>Author individual Teach-back section: provide a line-level code explanation and state the loop invariant for the explicit stack.</li></ul> |

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
