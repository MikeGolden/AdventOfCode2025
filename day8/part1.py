from typing import List, Tuple
from itertools import combinations


class UnionFind:
    def __init__(self, n: int):
        self.parent = list(range(n))
        self.size = [1] * n

    def find(self, x: int) -> int:
        if self.parent[x] != x:
            self.parent[x] = self.find(self.parent[x])
        return self.parent[x]

    def union(self, a: int, b: int) -> None:
        ra, rb = self.find(a), self.find(b)
        if ra == rb:
            return
        if self.size[ra] < self.size[rb]:
            ra, rb = rb, ra
        self.parent[rb] = ra
        self.size[ra] += self.size[rb]


def read_input(path: str) -> List[Tuple[int, int, int]]:
    with open(path, "r", encoding="utf-8") as f:
        return [tuple(map(int, line.strip().split(","))) for line in f if line.strip()]


def squared_distance(a: Tuple[int, int, int], b: Tuple[int, int, int]) -> int:
    return sum((a[i] - b[i]) ** 2 for i in range(3))


def main() -> None:
    points = read_input("input.txt")
    n = len(points)

    edges = []
    for (i, p1), (j, p2) in combinations(enumerate(points), 2):
        edges.append((squared_distance(p1, p2), i, j))

    edges.sort(key=lambda e: e[0])

    uf = UnionFind(n)

    for _, a, b in edges[:1000]:
        uf.union(a, b)

    component_sizes = {}
    for i in range(n):
        root = uf.find(i)
        component_sizes[root] = component_sizes.get(root, 0) + 1

    largest = sorted(component_sizes.values(), reverse=True)[:3]
    result = largest[0] * largest[1] * largest[2]
    print(result)


if __name__ == "__main__":
    main()
