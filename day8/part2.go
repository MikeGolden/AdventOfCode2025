package main

import (
	"bufio"
	"fmt"
	"os"
	"sort"
)

type Point struct {
	x, y, z int
}

type Edge struct {
	dist2 int
	a, b  int
}

type UnionFind struct {
	parent     []int
	rank       []int
	components int
}

func NewUnionFind(n int) *UnionFind {
	p := make([]int, n)
	r := make([]int, n)
	for i := range p {
		p[i] = i
	}
	return &UnionFind{p, r, n}
}

func (uf *UnionFind) Find(x int) int {
	if uf.parent[x] != x {
		uf.parent[x] = uf.Find(uf.parent[x])
	}
	return uf.parent[x]
}

func (uf *UnionFind) Union(a, b int) bool {
	ra, rb := uf.Find(a), uf.Find(b)
	if ra == rb {
		return false
	}
	if uf.rank[ra] < uf.rank[rb] {
		uf.parent[ra] = rb
	} else if uf.rank[ra] > uf.rank[rb] {
		uf.parent[rb] = ra
	} else {
		uf.parent[rb] = ra
		uf.rank[ra]++
	}
	uf.components--
	return true
}

func main() {
	file, _ := os.Open("input.txt")
	defer file.Close()

	var points []Point
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		var x, y, z int
		fmt.Sscanf(scanner.Text(), "%d,%d,%d", &x, &y, &z)
		points = append(points, Point{x, y, z})
	}

	var edges []Edge
	for i := 0; i < len(points); i++ {
		for j := i + 1; j < len(points); j++ {
			dx := points[i].x - points[j].x
			dy := points[i].y - points[j].y
			dz := points[i].z - points[j].z
			edges = append(edges, Edge{
				dx*dx + dy*dy + dz*dz, i, j,
			})
		}
	}

	sort.Slice(edges, func(i, j int) bool {
		return edges[i].dist2 < edges[j].dist2
	})

	uf := NewUnionFind(len(points))

	for _, e := range edges {
		if uf.Union(e.a, e.b) && uf.components == 1 {
			fmt.Println(points[e.a].x * points[e.b].x)
			return
		}
	}
}
