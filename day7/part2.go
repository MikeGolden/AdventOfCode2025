package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

type Pos struct {
	R, C int
}

func readInput(path string) ([]string, error) {
	f, err := os.Open(path)
	if err != nil {
		return nil, err
	}
	defer f.Close()

	var grid []string
	scanner := bufio.NewScanner(f)
	for scanner.Scan() {
		line := strings.TrimSpace(scanner.Text())
		if line != "" {
			grid = append(grid, line)
		}
	}
	return grid, scanner.Err()
}

func countTimeLines(grid []string) int64 {
	rows, cols := len(grid), len(grid[0])
	startCol := strings.Index(grid[0], "S")

	dp := map[Pos]int64{{0, startCol}: 1}
	var total int64

	for r := 0; r < rows; r++ {
		next := make(map[Pos]int64)

		for p, count := range dp {
			if p.R != r {
				continue
			}

			if p.C < 0 || p.C >= cols || p.R == rows-1 {
				total += count
				continue
			}

			if grid[p.R][p.C] == '^' {
				next[Pos{p.R + 1, p.C - 1}] += count
				next[Pos{p.R + 1, p.C + 1}] += count
			} else {
				next[Pos{p.R + 1, p.C}] += count
			}
		}

		dp = next
	}

	return total
}

func main() {
	grid, err := readInput("input.txt")
	if err != nil {
		panic(err)
	}
	fmt.Println(countTimeLines(grid))
}
