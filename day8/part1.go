package main

import (
	"bufio"
	"fmt"
	"os"
	"sort"
)

type Point struct {
	x, y, z int64
}

type Edge struct {
	dist int64
	a, b int
}

type UnionFind struct {
	parent []int
	size   []int
}

func NewUnionFind(n int) *UnionFind {
	p := make([]int, n)
	s := make([]int, n)
	for i := 0; i < n; i++ {
		p[i] = i
		s[i] = 1
	}
	return &UnionFind{p, s}
}

func (uf *UnionFind) Find(x int) int {
	if uf.parent[x] != x {
		uf.parent[x] = uf.Find(uf.parent[x])
	}
	return uf.parent[x]
}

func (uf *UnionFind) Union(a, b int) {
	ra, rb := uf.Find(a), uf.Find(b)
	if ra == rb {
		return
	}
	if uf.size[ra] < uf.size[rb] {
		ra, rb = rb, ra
	}
	uf.parent[rb] = ra
	uf.size[ra] += uf.size[rb]
}

func main() {
	file, err := os.Open("input.txt")
	if err != nil {
		panic(err)
	}
	defer file.Close()

	var points []Point
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		var p Point
		fmt.Sscanf(scanner.Text(), "%d,%d,%d", &p.x, &p.y, &p.z)
		points = append(points, p)
	}

	var edges []Edge
	n := len(points)
	for i := 0; i < n; i++ {
		for j := i + 1; j < n; j++ {
			dx := points[i].x - points[j].x
			dy := points[i].y - points[j].y
			dz := points[i].z - points[j].z
			dist := dx*dx + dy*dy + dz*dz
			edges = append(edges, Edge{dist, i, j})
		}
	}

	sort.Slice(edges, func(i, j int) bool {
		return edges[i].dist < edges[j].dist
	})

	uf := NewUnionFind(n)
	for i := 0; i < 1000; i++ {
		uf.Union(edges[i].a, edges[i].b)
	}

	component := make(map[int]int)
	for i := 0; i < n; i++ {
		component[uf.Find(i)]++
	}

	var sizes []int
	for _, s := range component {
		sizes = append(sizes, s)
	}
	sort.Sort(sort.Reverse(sort.IntSlice(sizes)))

	fmt.Println(sizes[0] * sizes[1] * sizes[2])
}
