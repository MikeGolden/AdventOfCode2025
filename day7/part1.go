package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

func readInput(path string) ([]string, error) {
	file, err := os.Open(path)
	if err != nil {
		return nil, err
	}
	defer file.Close()

	var grid []string
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := strings.TrimSpace(scanner.Text())
		if line != "" {
			grid = append(grid, line)
		}
	}
	return grid, scanner.Err()
}

func countSplits(grid []string) int {
	rows := len(grid)
	cols := len(grid[0])

	startCol := strings.Index(grid[0], "S")
	active := map[int]bool{startCol: true}
	splits := 0

	for r := 1; r < rows; r++ {
		next := make(map[int]bool)

		for c := range active {
			if c < 0 || c >= cols {
				continue
			}

			if grid[r][c] == '^' {
				splits++
				if c-1 >= 0 {
					next[c-1] = true
				}
				if c+1 < cols {
					next[c+1] = true
				}
			} else {
				next[c] = true
			}
		}

		if len(next) == 0 {
			break
		}
		active = next
	}

	return splits
}

func main() {
	grid, err := readInput("input.txt")
	if err != nil {
		panic(err)
	}
	fmt.Println(countSplits(grid))
}
