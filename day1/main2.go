package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"
)

func zerosDuring(pos, dist, dir int) int {
	var firstStep int
	if dir == 1 {
		firstStep = (100 - pos) % 100
	} else {
		firstStep = pos % 100
	}

	if firstStep == 0 {
		if dist < 100 {
			return 0
		}
		return 1 + (dist-100)/100
	}
	if dist < firstStep {
		return 0
	}
	return 1 + (dist-firstStep)/100
}

func main() {
	f, err := os.Open("input.txt")
	if err != nil {
		log.Fatalf("open input.txt: %v", err)
	}
	defer f.Close()

	scanner := bufio.NewScanner(f)
	pos := 50
	countZero := 0

	for scanner.Scan() {
		line := strings.TrimSpace(scanner.Text())
		if line == "" {
			continue
		}
		dirChar := line[0]
		dist, err := strconv.Atoi(line[1:])
		if err != nil {
			log.Fatalf("bad line %q: %v", line, err)
		}
		dir := 1
		if dirChar == 'L' {
			dir = -1
		}

		countZero += zerosDuring(pos, dist, dir)
		pos = (pos + dir*dist) % 100
		if pos < 0 {
			pos += 100
		}
	}

	if err := scanner.Err(); err != nil {
		log.Fatal(err)
	}
	fmt.Println(countZero)
}