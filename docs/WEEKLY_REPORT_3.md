# DSA Final Project - Weekly Report 3

### Group Progress Summary (Week 3)
This week, we successfully finished the testing and benchmarking phase for both BFS and DFS. We tested our algorithms on real-world datasets (Social Network and Course Prerequisites) to make sure they work correctly. 
To test the $O(V+E)$ time limit, we generated massive adversarial graphs (a 500,000-node Star Graph and a 200,000-node Linear Graph). We both noticed that dynamic memory allocation slows down standard structures like `std::queue` and `std::stack`. By switching to pre-allocated `std::vector` with `reserve(V)`, we made both algorithms run much faster.

### Week 3 Objectives & Task Allocation
| Member | Assigned Tasks (Week 3) |
| :--- | :--- |
| **Vũ Thành Huy** | - Create a Social Network dataset and test BFS shortest path.<br>- Benchmark BFS on a massive Star Graph to test $O(V+E)$.<br>- Optimize queue memory and document the time difference. |
| **Lê Thiện Luân** | - Create a Course Prerequisites dataset and test DFS cycle detection.<br>- Benchmark DFS on a massive Linear Graph to test $O(V+E)$.<br>- Optimize stack memory and document the time difference. |

### Syllabus Alignment Mapping (Week 3)
| Topic | Implementation |
| :--- | :--- |
| **Graphs & Traversal** | BFS used for shortest path; DFS used for cycle detection. |
| **Complexity** | Stress-tested $O(V+E)$ with 500k and 200k nodes. |
| **Memory Optimization** | Used `reserve(V)` to eliminate dynamic allocation overhead. |

### Key Commits (Week 3)
- `137a149` Tối ưu DFS, thêm dữ liệu thực tế, adversarial test và đo hiệu năng DFS cho tuần 3
- `ce68a96` Sửa lỗi O(V^2) gây treo máy trong hàm buildStarGraph
- `56f0d5c` Thêm dữ liệu thực tế, adversarial test và đo hiệu năng BFS cho Tuần 3

---

### Individual Teach-back Sections

#### Week 3 (Vũ Thành Huy) - BFS & Optimization

**1. Real-World Dataset: Social Network**
I built a social network graph with 6 people: `Huy`, `Luan`, `Kha`, `Alice`, `Bob`, `Charlie`. I ran my BFS code to find the shortest connection between `Huy` and `Bob`. The code returned `Huy -> Luan -> Alice -> Bob` (3 edges). Since BFS expands level by level, the first time it reaches a target in an unweighted graph, it is guaranteed to be the shortest path. 
*(Edge cases tested: Empty graph, same source/target, and unreachable nodes all return safely).*

**2. Benchmark & Memory Optimization: Star Graph**
To push BFS to its limit, I tried to build a Star Graph with $V = 500,000$. Here, I ran into a huge $O(V^2)$ bug. The AI suggested building the graph by calling `addEdge("0", to_string(i))` in a loop. However, my `addEdge` function uses `getIndex()` which does a linear string search. Running this 500,000 times caused the program to freeze. I fixed this by bypassing the string search and pushing directly into the adjacency list: `g.adj[0].push_back(i)`.

Once the graph was built, I tested standard `std::queue`. It took **18,929 $\mu$s**. The problem is that `std::queue` allocates memory in chunks dynamically, which is slow for 500,000 instant pushes. 
I optimized this by replacing it with a `std::vector<int>` and calling `reserve(V)` to allocate all memory at once. This dropped the execution time to **5,883 $\mu$s** (over 3x faster), proving that avoiding dynamic allocation overhead keeps the algorithm close to the theoretical $O(V+E)$.

![Benchmark Results](images/week3_bfs_benchmark.png)

### Week 3 (Lê Thiện Luân) - DFS & Optimization

### 1. Real-World Dataset (Course Prerequisites) & Cycle Detection

I constructed a course prerequisite graph representing academic module dependencies: `NMLT`, `CTDL`, `ThuatToan`, `CSDL`, `HDH`, `AI`, `CNPM`, and `KLTN`.

* **Cycle Detection Search:** I applied the Depth-First Search algorithm with an `inStack` tracking array (`iterativeDFS`) to detect potential conflicts (circular dependencies or deadlocks) within the curriculum structure.
* **Theoretical Justification:** DFS is ideal for dependency analysis and cycle detection because it explores deeply along each branch using an active call/execution stack. By maintaining a boolean `inStack` state for vertices on the current path, any edge pointing to a node currently in `inStack` immediately signifies a Back-Edge (circular dependency $KLTN \rightarrow NMLT$), violating the Directed Acyclic Graph (DAG) property.
* **Scenarios Handled (Verified in Test 3):**
  * *Valid DAG (Scenario A):* Verified that a standard curriculum flow operates without false positive cycle flags.
  * *Circular Dependency / Deadlock (Scenario B):* Successfully detected circular dependencies ($KLTN \rightarrow NMLT$) and identified the exact back-edge causing the conflict.

### 2. Benchmark & Optimization (Empirical vs. Theoretical)

To test the $\mathcal{O}(V + E)$ theoretical complexity under extreme conditions, I designed an adversarial "Linear Graph" ($V = 200,000$) where edges form a single chain $0 \rightarrow 1 \rightarrow 2 \rightarrow \dots \rightarrow V-1$. This topology forces maximum recursion depth and memory usage, expanding the stack to $V$ frames.

**Optimization Attempt:** While standard `std::stack<pair<int, int>>` (backed by `std::deque`) provides theoretical $\mathcal{O}(1)$ push/pop operations, its underlying segmented/chunked dynamic memory allocation introduces notable overhead during deep traversals. I optimized this by replacing `std::stack` with a pre-allocated `std::vector<pair<int, int>>` acting as a stack with `st.reserve(V)` to ensure contiguous memory allocation.

#### Results:

| Algorithm | Data Structure | Maximum Stack Depth | Execution Time | Evaluation |
| :--- | :--- | :--- | :--- | :--- |
| **Standard DFS** | `std::stack<pair<int, int>>` | 200,000 / 200,000 | 21056 $\mu s$ | High overhead due to dynamic memory chunk allocations in `std::deque`. |
| **Optimized DFS** | `std::vector<pair<int, int>>` | 200,000 / 200,000 | 14054 $\mu s$ | Performance improved. Pre-allocating contiguous memory via `reserve(V)` eliminated dynamic allocation bottlenecks, matching empirical performance with theoretical $\mathcal{O}(V + E)$ expectations. |

![Benchmark Results](images/Week3_DFS_benchmark.jpg)


### Plan for Week 4 (Finalize, Reflect & Prepare Defense)
Next week is the final phase, focusing strictly on project wrap-up, limitations analysis, and oral defense preparation. No new features will be implemented.

| Member | Planned Tasks (Week 4) |
| :--- | :--- |
| **Vũ Thành Huy** | - Finalize time/space complexity derivation $O(V+E)$ and document project limitations.<br>- Draft 10 anticipated defense questions & answers (Self-quiz).<br>- Write personal reflection (hardest challenge, what to change with 2 more weeks).<br>- Compile the final AI usage log. |
| **Lê Thiện Luân** | - Map every numbered project requirement to its exact code location.<br>- Draft 10 anticipated defense questions & answers (Self-quiz).<br>- Write personal reflection (hardest challenge, what to change with 2 more weeks).<br>- Compile the final AI usage log. |
