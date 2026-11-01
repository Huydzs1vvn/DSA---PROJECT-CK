# DSA Final Project - Weekly Report 4 (Final)

###  WEEK 4 TASK ALLOCATION (Finalize, Reflect & Prepare Defense)
Next week is the final phase, focusing strictly on project wrap-up, limitations analysis, and oral defense preparation. No new features will be implemented.

| Member | Planned Tasks (Week 4) |
| :--- | :--- |
| **Vũ Thành Huy** | - **Create visualization diagram (numbered nodes) showing the traversal order for at least one example graph (per teacher's request).**<br>- Finalize time/space complexity derivation $O(V + E)$ and document project limitations.<br>- Draft 10 anticipated defense questions & answers (Self-quiz).<br>- Write personal reflection (hardest challenge, what to change with 2 more weeks).<br>- Compile the final AI usage log. |
| **Lê Thiện Luân** | - Map every numbered project requirement to its exact code location.<br>- Draft 10 anticipated defense questions & answers (Self-quiz).<br>- Write personal reflection (hardest challenge, what to change with 2 more weeks).<br>- Compile the final AI usage log. |

---

### 1. Visualization of Traversal Order (Numbered Nodes)
*(Per teacher's feedback in Week 3, below is the numbered traversal order of our BFS algorithm starting from node A)*

![BFS Traversal Order](images/DoThi.png)

*Traversal Order details:*
- BFS visits A (1), then explores its neighbors B (2) and C (3).
- Moving to queue expansion, it explores the neighbors of B, which is D (4), and the neighbors of C, which is F (5).
- Finally, D's unvisited neighbor is E (6). 
- Thus, the visit sequence is: `A (1) -> B (2) -> C (3) -> D (4) -> F (5) -> E (6)`.

---

### 2. Final Project Limitations
Our BFS and DFS implementations meet all core DSA requirements, but have the following real-world limitations:
- **Scalability of String Handling:** While the traversal itself runs in $O(V+E)$, the use of string-based IDs and the `getIndex()` lookup helper in initial construction can become a bottleneck on highly dense, huge networks if not completely bypassed. 
- **In-Memory Limits:** Huge data structures like 500k-node Star Graph or 200k-node Linear Graph consume massive RAM because we do everything in-memory. In real apps, this must be handled by off-loading to db/chunks.
- **Unweighted-Only for BFS shortest paths:** Our BFS correctly resolves shortest paths based on edge counts. However, it cannot handle weighted networks (e.g. travel time, distance), which would require Dijkstra.
