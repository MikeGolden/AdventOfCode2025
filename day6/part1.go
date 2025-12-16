package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

func readInput(path string) ([]string, error) {
	file, err := os.Open(path)
	if err != nil {
		return nil, err
	}
	defer file.Close()

	var lines []string
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		lines = append(lines, scanner.Text())
	}
	return lines, scanner.Err()
}

func splitBlocks(lines []string) [][][]rune {
	width := 0
	for _, l := range lines {
		if len(l) > width {
			width = len(l)
		}
	}

	grid := make([][]rune, len(lines))
	for i, l := range lines {
		row := []rune(l)
		for len(row) < width {
			row = append(row, ' ')
		}
		grid[i] = row
	}

	used := make([]bool, width)
	for c := 0; c < width; c++ {
		for r := range grid {
			if grid[r][c] != ' ' {
				used[c] = true
				break
			}
		}
	}

	var blocks [][][]rune
	start := -1

	for i := 0; i <= width; i++ {
		active := i < width && used[i]
		if active && start == -1 {
			start = i
		} else if !active && start != -1 {
			block := make([][]rune, len(grid))
			for r := range grid {
				block[r] = grid[r][start:i]
			}
			blocks = append(blocks, block)
			start = -1
		}
	}

	return blocks
}

func solveBlock(block [][]rune) int64 {
	var numbers []int64
	var op rune

	for _, row := range block {
		s := strings.TrimSpace(string(row))
		if s == "" {
			continue
		}
		if s == "+" || s == "*" {
			op = rune(s[0])
		} else {
			n, err := strconv.ParseInt(s, 10, 64)
			if err != nil {
				panic(err)
			}
			numbers = append(numbers, n)
		}
	}

	if op == '+' {
		var sum int64
		for _, n := range numbers {
			sum += n
		}
		return sum
	}

	result := int64(1)
	for _, n := range numbers {
		result *= n
	}
	return result
}

func main() {
	lines, err := readInput("input.txt")
	if err != nil {
		panic(err)
	}

	blocks := splitBlocks(lines)

	var total int64
	for _, b := range blocks {
		total += solveBlock(b)
	}

	fmt.Println(total)
}