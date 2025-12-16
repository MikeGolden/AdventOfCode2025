package main

import (
	"bufio"
	"fmt"
	"os"
	"sort"
	"strconv"
	"strings"
)

type Range struct {
	Start int
	End   int
}

func parseRanges(path string) ([]Range, error) {
	file, err := os.Open(path)
	if err != nil {
		return nil, err
	}
	defer file.Close()

	var ranges []Range
	scanner := bufio.NewScanner(file)

	for scanner.Scan() {
		line := strings.TrimSpace(scanner.Text())
		if line == "" {
			break
		}
		parts := strings.Split(line, "-")
		start, _ := strconv.Atoi(parts[0])
		end, _ := strconv.Atoi(parts[1])
		ranges = append(ranges, Range{Start: start, End: end})
	}

	return ranges, scanner.Err()
}

func countUnionSize(ranges []Range) int {
	if len(ranges) == 0 {
		return 0
	}

	sort.Slice(ranges, func(i, j int) bool {
		return ranges[i].Start < ranges[j].Start
	})

	total := 0
	currentStart := ranges[0].Start
	currentEnd := ranges[0].End

	for _, r := range ranges[1:] {
		if r.Start > currentEnd+1 {
			total += currentEnd - currentStart + 1
			currentStart = r.Start
			currentEnd = r.End
		} else if r.End > currentEnd {
			currentEnd = r.End
		}
	}

	total += currentEnd - currentStart + 1
	return total
}

func main() {
	ranges, err := parseRanges("input.txt")
	if err != nil {
		panic(err)
	}

	fmt.Println(countUnionSize(ranges))
}
