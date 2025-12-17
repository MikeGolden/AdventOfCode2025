import * as fs from "fs";

type Point = { x: number; y: number; z: number };
type Edge = { dist2: number; a: number; b: number };

class UnionFind {
  parent: number[];
  rank: number[];
  components: number;

  constructor(n: number) {
    this.parent = Array.from({ length: n }, (_, i) => i);
    this.rank = new Array(n).fill(0);
    this.components = n;
  }

  find(x: number): number {
    if (this.parent[x] !== x) {
      this.parent[x] = this.find(this.parent[x]);
    }
    return this.parent[x];
  }

  union(a: number, b: number): boolean {
    const ra = this.find(a);
    const rb = this.find(b);
    if (ra === rb) return false;

    if (this.rank[ra] < this.rank[rb]) this.parent[ra] = rb;
    else if (this.rank[ra] > this.rank[rb]) this.parent[rb] = ra;
    else {
      this.parent[rb] = ra;
      this.rank[ra]++;
    }

    this.components--;
    return true;
  }
}

const input = fs.readFileSync("input.txt", "utf-8").trim().split("\n");

const points: Point[] = input.map((line) => {
  const [x, y, z] = line.split(",").map(Number);
  return { x, y, z };
});

const edges: Edge[] = [];

for (let i = 0; i < points.length; i++) {
  for (let j = i + 1; j < points.length; j++) {
    const dx = points[i].x - points[j].x;
    const dy = points[i].y - points[j].y;
    const dz = points[i].z - points[j].z;
    edges.push({ dist2: dx * dx + dy * dy + dz * dz, a: i, b: j });
  }
}

edges.sort((a, b) => a.dist2 - b.dist2);

const uf = new UnionFind(points.length);

for (const e of edges) {
  if (uf.union(e.a, e.b) && uf.components === 1) {
    console.log(points[e.a].x * points[e.b].x);
    break;
  }
}
