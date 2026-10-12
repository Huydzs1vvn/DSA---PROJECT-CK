(Week 1)**

| Member | Assigned Tasks | Completion Rate |
| :--- | :--- | :--- |
| **Vũ Thành Huy** |  Draw test graph, structure GitHub repository, research BFS & Adjacency List, hand-trace shortest path algorithm, compile report. | 100% |
| **Lê Thiện Luân** | research Iterative DFS using Stack, hand-trace Cycle Detection algorithm. | 100% |

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
