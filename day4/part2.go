package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"strings"
)

func loadGrid(path string) ([][]rune, error) {
	f, err := os.Open(path)
	if err != nil {
		return nil, err
	}
	defer f.Close()

	var grid [][]rune
	scanner := bufio.NewScanner(f)
	for scanner.Scan() {
		line := strings.TrimRight(scanner.Text(), "\n")
		if strings.TrimSpace(line) == "" {
			continue
		}
		grid = append(grid, []rune(line))
	}
	return grid, scanner.Err()
}

func findAccessible(grid [][]rune) [][2]int {
	if len(grid) == 0 {
		return nil
	}
	dirs := [8][2]int{
		{-1, -1},
		{-1, 0},
		{-1, 1},
		{0, -1},
		{0, 1},
		{1, -1},
		{1, 0},
		{1, 1},
	}
	var res [][2]int
	for r := range grid {
		for c := range grid[r] {
			if grid[r][c] != '@' {
				continue
			}
			adj := 0
			for _, d := range dirs {
				nr := r + d[0]
				nc := c + d[1]
				if nr >= 0 && nr < len(grid) && nc >= 0 && nc < len(grid[nr]) {
					if grid[nr][nc] == '@' {
						adj++
						if adj >= 4 {
							break
						}
					}
				}
			}
			if adj < 4 {
				res = append(res, [2]int{r, c})
			}
		}
	}
	return res
}

func removeUntilStable(grid [][]rune) int {
	total := 0
	for {
		toRemove := findAccessible(grid)
		if len(toRemove) == 0 {
			break
		}
		for _, rc := range toRemove {
			r, c := rc[0], rc[1]
			if grid[r][c] == '@' {
				grid[r][c] = '.'
				total++
			}
		}
	}
	return total
}

func main() {
	grid, err := loadGrid("input.txt")
	if err != nil {
		log.Fatalf("failed to read input.txt: %v", err)
	}
	result := removeUntilStable(grid)
	fmt.Println(result)
}
