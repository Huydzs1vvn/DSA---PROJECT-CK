(Week 1)**

| Member | Assigned Tasks | Completion Rate |
| :--- | :--- | :--- |
| **Vũ Thành Huy** |  Draw test graph, structure GitHub repository, research BFS & Adjacency List, hand-trace shortest path algorithm, compile report. | 100% |
| **Lê Thiện Luân** | research Iterative DFS using Stack, hand-trace Cycle Detection algorithm. | 100% |

## Individual Teach-back (Vũ Thành Huy)

**1. Hand-traceable Example (BFS & Shortest Path)**
*Graph traversal from A to F using a Queue to find the shortest path.*

| Step | Current Node (Dequeue) | Queue State | Visited Nodes | Parent Array (Path Tracking) |
| :--- | :--- | :--- | :--- | :--- |
| 0 | Initialization | [A] | {A} | Parent[A] = null |
| 1 | A | [B, C] | {A, B, C} | Parent[B] = A, Parent[C] = A |
| 2 | B | [C, D] | {A, B, C, D} | Parent[D] = B (C is unchanged) |
| 3 | C | [D, F] | {A, B, C, D, F} | Parent[F] = C |
| 4 | D | [F, E] | {A, B, C, D, F, E} | Parent[E] = D (F is unchanged) |
| 5 | F | [E] | {A, B, C, D, F, E} | (No unvisited neighbors) |
| 6 | E | [] | {A, B, C, D, F, E} | (No unvisited neighbors) |

**Conclusion:** The shortest path from A to F traced backward is: F <- C <- A.

**2. Why I Chose Adjacency List for BFS**
In this project, we decided to use an Adjacency List instead of an Adjacency Matrix to represent the graph. For my task (BFS and Shortest Path), this data structure provides three major advantages:
* **Optimal Time Complexity $O(V + E)$:** BFS requires checking all neighbors of a node. With an Adjacency Matrix, I would have to check all $V$ nodes every time (taking $O(V^2)$). However, an Adjacency List only stores actual connected neighbors. This allows the BFS loop to run much faster, achieving the optimal $O(V + E)$ time complexity.
* **Memory Efficiency for Sparse Graphs:** Real-world datasets like social networks or road maps (which I will test in Week 3) are "sparse graphs" — meaning each node only connects to a few others. An Adjacency Matrix would waste huge memory ($O(V^2)$) storing mostly zeros. An Adjacency List only takes $O(V + E)$ space.
* **Cleaner Code for Path Tracking:** When using BFS with a `std::queue` to track the `parent[]` array, iterating over neighbors using a for-each loop on an Adjacency List makes the logic simple and bug-free.


## Individual Teach-back (Lê Thiện Luân)

**1. Hand-traceable Example (Iterative DFS & Cycle Detection)**
*Graph traversal from A to F using an explicit Stack and detecting a cycle.*

| Step | Current Vertex | Edge Traversed | Action Taken | Stack State | Visited Set |
| :---: | :--- | :--- | :--- | :--- | :--- |
| **1** | A | Start | Root push A | `[A]` | `{A}` |
| **2** | B | A → B | Push B (not visited) | `[A, B]` | `{A, B}` |
| **3** | D | B → D | Push D (not visited) | `[A, B, D]` | `{A, B, D}` |
| **4** | - | D → B | Skip B (already visited) | `[A, B, D]` | `{A, B, D}` |
| **5** | E | D → E | Push E (not visited) | `[A, B, D, E]` | `{A, B, D, E}` |
| **6** | F | E → F | Push F (not visited) | `[A, B, D, E, F]` | `{A, B, D, E, F}` |
| **7** | - | Backtrack | Pop F, E, D, B (no outgoing) | `[A]` | `{A, B, D, E, F}` |
| **8** | C | A → C | Push C (tree edge) | `[A, C]` | `{A, B, D, E, F, C}` |
| **9** | - | C → F | Skip F (already visited) | `[A, C]` | `{A, B, D, E, F, C}` |
| **10** | - | Backtrack | Complete C and A | `[]` | `{A, B, D, E, F, C}` |

**Conclusion:** The discovery order is A → B → D → E → F → C, successfully identifying the back-edge (D → B) as a **Cycle** and edge (C → F) as a **Cross edge**.

**2. Why I Chose Explicit Stack & Adjacency List for DFS**
In this project, we decided to use an Adjacency List and an explicit stack instead of recursion. For my task (Iterative DFS and Cycle Detection), this approach provides three major advantages:
* **Optimal Time Complexity $O(V + E)$:** Checking neighbors via an Adjacency List takes $O(1)$ per edge, ensuring our Iterative DFS runs in optimal $O(V + E)$ time without the overhead of an Adjacency Matrix.
* **Explicit Stack Control:** Using `std::stack` prevents recursion and stack overflow risks on large graphs (tested in Week 3), while giving us strict control over node states (White/Gray/Black) to reliably catch back-edges.
* **Cleaner Code for State Tracking:** Managing the stack and visited states explicitly makes the logic clean, modular, and easy to trace against our hand-drawn test case.
