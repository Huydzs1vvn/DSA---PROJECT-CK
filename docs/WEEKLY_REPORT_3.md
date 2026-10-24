### Plan for Week 3 (Features, Experiments & Analysis)
Next week, we will focus on applying our DFS/BFS toolkit to real-world datasets and empirically proving our theoretical complexity analysis.

| Member | Planned Tasks (Week 3) |
| :--- | :--- |
| **Vũ Thành Huy** | - Construct a real-world unweighted dataset (e.g., a simplified road map or social network).<br>- Run pathfinding experiments comparing BFS and DFS on the dataset to prove why BFS yields shortest paths while DFS does not.<br>- Benchmark BFS execution time on large-scale random graphs to empirically validate the $O(V + E)$ boundary. |
| **Lê Thiện Luân** | - Construct a real-world dataset representing dependencies (e.g., course prerequisites or task scheduling).<br>- Apply the Cycle Detection DFS algorithm to find conflicts (circular dependencies) in the dataset.<br>- Benchmark DFS execution time and memory (maximum stack depth) on large-scale inputs to validate the $O(V + E)$ limits. |
