// Select exactly 2 digits per bank to maximize the resulting 2-digit number
// Usage: go run part1.go

package main

import (
	"bufio"
	"fmt"
	"log"
	"math/big"
	"os"
	"strings"
)

// BankChoice holds the best choice for a bank.
type BankChoice struct {
	Value      int    // two-digit integer 10*tens + units
	Digits     string // the chosen two digits concatenated
	TensIndex  int    // index of tens digit (0-based)
	UnitsIndex int    // index of units digit (0-based)
}

// besTwo picks the best pair (i<j) maximizing 10*digit[i] + digit[j].
func bestTwo(line string) (BankChoice, error) {
	s := strings.TrimSpace(line)
	if s == "" {
		return BankChoice{Value: 0, Digits: "", TensIndex: -1, UnitsIndex: -1}, nil
	}
	n := len(s)
	// Validate digits
	for _, ch := range s {
		if ch < '0' || ch > '9' {
			return BankChoice{}, fmt.Errorf("non-digit character in bank: %q", ch)
		}
	}
	// suffix max digit and its rightmost index
	suffixMax := make([]byte, n)
	suffixIdx := make([]int, n)
	suffixMax[n-1] = s[n-1]
	suffixIdx[n-1] = n - 1
	for i := n - 2; i >= 0; i-- {
		right := s[i+1]
		if right >= suffixMax[i+1] {
			suffixMax[i] = right
			suffixIdx[i] = i + 1
		} else {
			suffixMax[i] = suffixMax[i+1]
			suffixIdx[i] = suffixIdx[i+1]
		}
	}
	bestVal := -1
	bestI, bestJ := -1, -1
	for i := 0; i < n-1; i++ {
		tens := int(s[i] - '0')
		units := int(suffixMax[i] - '0')
		val := tens*10 + units
		if val > bestVal {
			bestVal = val
			bestI = i
			bestJ = suffixIdx[i]
		}
	}
	if bestI < 0 {
		// not enough digits
		return BankChoice{Value: 0, Digits: "", TensIndex: -1, UnitsIndex: -1}, nil
	}
	digits := fmt.Sprintf("%c%c", s[bestI], s[bestJ])
	return BankChoice{Value: bestVal, Digits: digits, TensIndex: bestI, UnitsIndex: bestJ}, nil
}

func main() {
	f, err := os.Open("input.txt")
	if err != nil {
		log.Fatalf("failed to open input.txt: %v", err)
	}
	defer f.Close()

	scanner := bufio.NewScanner(f)
	var choices []BankChoice
	lineNo := 0
	for scanner.Scan() {
		line := scanner.Text()
		lineNo++
		if strings.TrimSpace(line) == "" {
			continue
		}
		choice, err := bestTwo(line)
		if err != nil {
			log.Fatalf("error processing line %d: %v", lineNo, err)
		}
		choices = append(choices, choice)
	}
	if err := scanner.Err(); err != nil {
		log.Fatalf("scanner error: %v", err)
	}

	total := 0
	for i, ch := range choices {
		fmt.Printf("Bank %d:  value=%02d  digits=%s  indices=(%d, %d)\n", i+1, ch.Value, ch.Digits, ch.TensIndex, ch.UnitsIndex)
		total += ch.Value
	}
	fmt.Printf(strings.Repeat("-", 70))
	fmt.Printf("Total joltage (sum of per-bank maxima: %d\n", total)

	// Also show total as big.Int for completeness (should fit in int, but safe)
	bigTotal := big.NewInt(0)
	for _, ch := range choices {
		bigTotal.Add(bigTotal, big.NewInt(int64(ch.Value)))
	}
	_ = bigTotal // not printed to avoid duplication; available if needed
}
