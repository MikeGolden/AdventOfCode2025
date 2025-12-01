package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"
)

func main() {
	file, err := os.Open("input.txt")
	if err != nil {
		log.Fatalf("failed to open input.txt: %v", err)
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)

	pos := 50
	countZero := 0

	for scanner.Scan() {
		line := strings.TrimSpace(scanner.Text())
		if line == "" {
			continue
		}

		dir := line[0]
		dist, err := strconv.Atoi(line[1:])
		if err != nil {
			log.Fatalf("bad line %q: %v", line, err)
		}

		if dir == 'R' {
			pos = (pos + dist) % 100
		} else if dir == 'L' {
			pos = (pos - dist) % 100
		} else {
			log.Fatalf("bad direction %q in line %v", dir, line)
		}

		if pos == 0 {
			countZero++
		}
	}

	if err := scanner.Err(); err != nil {
		log.Fatal(err)
	}

	fmt.Println(countZero)
}
