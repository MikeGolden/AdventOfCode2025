package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"unicode"
)

func readInput(path string) ([]string, error) {
	f, err := os.Open(path)
	if err != nil {
		return nil, err
	}
	defer f.Close()

	var lines []string
	scanner := bufio.NewScanner(f)
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
	h := len(block)
	w := len(block[0])

	var numbers []int64
	var op rune

	for c := w - 1; c >= 0; c-- {
		var digits []rune
		for r := 0; r < h; r++ {
			ch := block[r][c]
			if unicode.IsDigit(ch) {
				digits = append(digits, ch)
			} else if ch == '+' || ch == '*' {
				op = ch
			}
		}

		if len(digits) > 0 {
			n, _ := strconv.ParseInt(string(digits), 10, 64)
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
