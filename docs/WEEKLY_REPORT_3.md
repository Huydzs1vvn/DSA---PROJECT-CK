### Plan for Week 3 (Features, Experiments & Analysis)
Next week, we will focus on applying our DFS/BFS toolkit to real-world datasets and empirically proving our theoretical complexity analysis.

| Member | Planned Tasks (Week 3) |
| :--- | :--- |
| **Vũ Thành Huy** | - Construct a real-world unweighted dataset (e.g., a simplified road map or social network).<br>- Run pathfinding experiments comparing BFS and DFS on the dataset to prove why BFS yields shortest paths while DFS does not.<br>- Benchmark BFS execution time on large-scale random graphs **and adversarial inputs (e.g., star graphs or highly dense graphs)** to empirically validate the $O(V + E)$ boundary.<br>- **Attempt at least one optimization for BFS (e.g., pre-allocating queue size or tweaking data structures) and document the performance data (kept or rejected).** |
| **Lê Thiện Luân** | - Construct a real-world dataset representing dependencies (e.g., course prerequisites or task scheduling).<br>- Apply the Cycle Detection DFS algorithm to find conflicts (circular dependencies) in the dataset.<br>- Benchmark DFS execution time and memory (maximum stack depth) on large-scale inputs **and adversarial inputs (e.g., degenerate linear graphs $1 \to 2 \to \dots \to V$)** to validate the $O(V + E)$ limits.<br>- **Attempt at least one optimization for DFS (e.g., adjacency list representations) and document the performance data (kept or rejected).** |


### Week 3 (Vũ Thành Huy) - BFS & Optimization

#### 1. Real-World Dataset (Social Network) & Edge Cases
I constructed a social network graph with 6 nodes: `Huy`, `Luan`, `Kha`, `Alice`, `Bob`, and `Charlie`. 
- **Shortest Path Search:** I used BFS to find the path from `Huy` to `Bob`. The algorithm successfully returned `Huy -> Luan -> Alice -> Bob` with a distance of 3 edges.
- **Theoretical Justification:** BFS guarantees the shortest path in unweighted graphs because it explores the graph strictly level-by-level. By the time the target node (`Bob`) is enqueued, the algorithm has implicitly checked all shorter possible paths.
- **Edge Cases Handled (Verified in Test 2):**
  - *Empty Graph (V=0):* Safely returns false without crashing.
  - *Source == Target (`A` to `A`):* Correctly returns path `A` with 0 edges.
  - *Unreachable Node:* Gracefully handles disconnected components and returns false.

#### 2. Benchmark & Optimization (Empirical vs. Theoretical)
To test the $O(V+E)$ theoretical complexity under extreme conditions, I designed an adversarial "Star Graph" ($V = 500,000$). This topology forces a massive queue expansion at Level 1, enqueueing 499,999 nodes instantly.

**Optimization Attempt:** 
While the standard `std::queue<int>` (implemented via `std::deque`) has an $O(1)$ theoretical enqueue time, its underlying chunk-based dynamic memory allocation causes severe empirical overhead when handling massive simultaneous pushes. I optimized this by replacing it with a pre-allocated `std::vector<int>` acting as a queue (`reserve(V)`).

**Results:**
| Algorithm | Data Structure | Execution Time | Evaluation |
| :--- | :--- | :--- | :--- |
| Standard BFS | `std::queue<int>` | 18,929 $\mu$s | High overhead due to dynamic memory allocation limits. |
| Optimized BFS | `std::vector<int>` | 5,883 $\mu$s | **KEPT**. Over 3x faster. Pre-allocating contiguous memory eliminated the chunk-allocation bottleneck, aligning empirical performance perfectly with the theoretical $O(V+E)$ ideal. |

![Benchmark Results](images/week3_bfs_benchmark.png)
