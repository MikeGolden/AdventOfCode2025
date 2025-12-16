package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

type Range struct {
	Start int
	End   int
}

func (r Range) Contains(value int) bool {
	return value >= r.Start && value <= r.End
}

func parseInput(path string) ([]Range, []int, error) {
	file, err := os.Open(path)
	if err != nil {
		return nil, nil, err
	}
	defer file.Close()

	var ranges []Range
	var ids []int
	scanner := bufio.NewScanner(file)

	parsingRanges := true

	for scanner.Scan() {
		line := strings.TrimSpace(scanner.Text())
		if line == "" {
			parsingRanges = false
			continue
		}

		if parsingRanges {
			parts := strings.Split(line, "-")
			start, _ := strconv.Atoi(parts[0])
			end, _ := strconv.Atoi(parts[1])
			ranges = append(ranges, Range{Start: start, End: end})
		} else {
			value, _ := strconv.Atoi(line)
			ids = append(ids, value)
		}
	}

	return ranges, ids, scanner.Err()
}

func countFreshIDs(ranges []Range, ids []int) int {
	count := 0
	for _, value := range ids {
		for _, r := range ranges {
			if r.Contains(value) {
				count++
				break
			}
		}
	}
	return count
}

func main() {
	ranges, ids, err := parseInput("input.txt")
	if err != nil {
		panic(err)
	}

	result := countFreshIDs(ranges, ids)
	fmt.Println(result)
}
