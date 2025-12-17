import * as fs from "fs";

type Point = [number, number, number];
type Edge = { dist: number; a: number; b: number };

class UnionFind {
  parent: number[];
  size: number[];

  constructor(n: number) {
    this.parent = Array.from({ length: n }, (_, i) => i);
    this.size = Array(n).fill(1);
  }

  find(x: number): number {
    if (this.parent[x] !== x) {
      this.parent[x] = this.find(this.parent[x]);
    }
    return this.parent[x];
  }

  union(a: number, b: number): void {
    let ra = this.find(a);
    let rb = this.find(b);
    if (ra === rb) return;
    if (this.size[ra] < this.size[rb]) [ra, rb] = [rb, ra];
    this.parent[rb] = ra;
    this.size[ra] += this.size[rb];
  }
}

const points: Point[] = fs
  .readFileSync("input.txt", "utf8")
  .trim()
  .split("\n")
  .map((l) => l.split(",").map(Number) as Point);

const edges: Edge[] = [];
for (let i = 0; i < points.length; i++) {
  for (let j = i + 1; j < points.length; j++) {
    const dx = points[i][0] - points[j][0];
    const dy = points[i][1] - points[j][1];
    const dz = points[i][2] - points[j][2];
    edges.push({ dist: dx * dx + dy * dy + dz * dz, a: i, b: j });
  }
}

edges.sort((a, b) => a.dist - b.dist);

const uf = new UnionFind(points.length);
for (let i = 0; i < 1000; i++) {
  uf.union(edges[i].a, edges[i].b);
}

const counts = new Map<number, number>();
for (let i = 0; i < points.length; i++) {
  const r = uf.find(i);
  counts.set(r, (counts.get(r) ?? 0) + 1);
}

const sizes = Array.from(counts.values()).sort((a, b) => b - a);
console.log(sizes[0] * sizes[1] * sizes[2]);
