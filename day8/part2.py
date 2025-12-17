from __future__ import annotations
from dataclasses import dataclass
from typing import List, Tuple


@dataclass(frozen=True)
class Point:
    x: int
    y: int
    z: int


class UnionFind:
    def __init__(self, size: int) -> None:
        self.parent = list(range(size))
        self.rank = [0] * size
        self.components = size

    def find(self, x: int) -> int:
        if self.parent[x] != x:
            self.parent[x] = self.find(self.parent[x])
        return self.parent[x]

    def union(self, a: int, b: int) -> bool:
        ra, rb = self.find(a), self.find(b)
        if ra == rb:
            return False

        if self.rank[ra] < self.rank[rb]:
            self.parent[ra] = rb
        elif self.rank[ra] > self.rank[rb]:
            self.parent[rb] = ra
        else:
            self.parent[rb] = ra
            self.rank[ra] += 1

        self.components -= 1
        return True


def read_input(path: str) -> List[Point]:
    points: List[Point] = []
    with open(path, "r", encoding="utf-8") as f:
        for line in f:
            x, y, z = map(int, line.strip().split(","))
            points.append(Point(x, y, z))
    return points


def solve(points: List[Point]) -> int:
    edges: List[Tuple[int, int, int]] = []

    for i in range(len(points)):
        for j in range(i + 1, len(points)):
            dx = points[i].x - points[j].x
            dy = points[i].y - points[j].y
            dz = points[i].z - points[j].z
            dist2 = dx * dx + dy * dy + dz * dz
            edges.append((dist2, i, j))

    edges.sort(key=lambda e: e[0])

    uf = UnionFind(len(points))

    for _, a, b in edges:
        if uf.union(a, b) and uf.components == 1:
            return points[a].x * points[b].x

    raise RuntimeError("Graph never fully connected")


if __name__ == "__main__":
    points = read_input("input.txt")
    print(solve(points))
