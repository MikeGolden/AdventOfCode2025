package main

import (
	"bufio"
	"fmt"
	"os"
)

type Point struct {
	X int
	Y int
}

func abs(x int) int {
	if x < 0 {
		return -x
	}
	return x
}

func main() {
	file, err := os.Open("input.txt")
	if err != nil {
		panic(err)
	}
	defer file.Close()

	points := make([]Point, 0)
	scanner := bufio.NewScanner(file)

	for scanner.Scan() {
		var x, y int
		fmt.Sscanf(scanner.Text(), "%d,%d", &x, &y)
		points = append(points, Point{x, y})
	}

	maxArea := 0
	n := len(points)

	for i := 0; i < n; i++ {
		for j := i + 1; j < n; j++ {
			area := (abs(points[i].X-points[j].X) + 1) * (abs(points[i].Y-points[j].Y) + 1)
			if area > maxArea {
				maxArea = area
			}
		}
	}

	fmt.Println(maxArea)
}
