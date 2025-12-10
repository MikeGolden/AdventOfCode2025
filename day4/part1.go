package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
)

// loadGrid reads a grid of characters from a file.
// Each non-empty line becomes a row in the grid.
func loadGrid(path string) ([][]rune, error) {
	file, err := os.Open(path)
	if err != nil {
		return nil, fmt.Errorf("could not open file %s: %w", path, err)
	}
	defer file.Close()

	var grid [][]rune
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := scanner.Text()
		if len(line) > 0 {
			grid = append(grid, []rune(line))
		}
	}

	if err := scanner.Err(); err != nil {
		return nil, fmt.Errorf("error reading file %s: %w", path, err)
	}

	return grid, nil
}

// countAccessibleRolls counts the number of '@' rolls that have fewer than 4
// adjacent '@' neighbors among the 8 surrounding cells.
func countAccessibleRolls(grid [][]rune) int {
	if len(grid) == 0 {
		return 0
	}

	h := len(grid)
	accessible := 0

	// All 8 directions around a given cell
	directions := []struct{ dr, dc int }{
		{-1, -1}, {-1, 0}, {-1, 1},
		{0, -1} /* {0, 0} */, {0, 1},
		{1, -1}, {1, 0}, {1, 1},
	}

	for r := 0; r < h; r++ {
		rowLen := len(grid[r])
		for c := 0; c < rowLen; c++ {
			if grid[r][c] != '@' {
				continue
			}

			adjacent := 0
			for _, dir := range directions {
				nr, nc := r+dir.dr, c+dir.dc

				// Check bounds for the new row
                if nr >= 0 && nr < h {
                    // Check bounds for the new column within its specific row
                    if nc >= 0 && nc < len(grid[nr]) {
                        if grid[nr][nc] == '@' {
                            adjacent++
                        }
                    }
                }
			}

			if adjacent < 4 {
				accessible++
			}
		}
	}

	return accessible
}

func main() {
	grid, err := loadGrid("input.txt")
	if err != nil {
		log.Fatalf("Failed to load grid: %v", err)
	}

	result := countAccessibleRolls(grid)
	fmt.Println(result)
}
