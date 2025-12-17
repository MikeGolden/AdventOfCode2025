## Correct problem model

- Each junction box = a node in 3D space
- Edge weight = **Euclidean distance**
- Sort **all possible pairs** by distance
- Process edges in ascending order
- For the **first 1000 edges**:
  - If the two nodes are already connected → do nothing
  - Otherwise → union their components
- After exactly 1000 processed edges:
  - Compute sizes of all connected components
  - Take the **three largest**
  - Multiply their sizes

This is **Kruskal-style union-find**, but:

- **NOT** a full MST
- We stop after a fixed number of edges
- Graph is complete (N² edges)

## **Engineering notes (important)**

- Use **squared distance** for sorting (no sqrt, preserves ordering)
- Use **Union-Find with path compression + union by size**
- Use **64-bit arithmetic** for distances
- This is O(N^2 \log N); acceptable for AoC-scale inputs