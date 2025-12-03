// Select exactly 12 digits per bank (k=12) to maximize the resulting number (preserving order).
// Usage: go run part2.go   (reads input.txt)

package main

import (
	"bufio"
	"fmt"
	"log"
	"math/big"
	"os"
	"strings"
)

const K = 12

type BankChoice struct {
	Value   *big.Int // big integer of chosen K digits
	Digits  string
	Indices []int
}

// bestK selects the lexicographically largest subsequence of length k using a monotonic stack.
func bestK(line string, k int) (BankChoice, error) {
	s := strings.TrimSpace(line)
	if s == "" {
		return BankChoice{Value: big.NewInt(0), Digits: "", Indices: nil}, nil
	}
	n := len(s)
	for _, ch := range s {
		if ch < '0' || ch > '9' {
			return BankChoice{}, fmt.Errorf("non-digit character found: %q", ch)
		}
	}
	if k == 0 {
		return BankChoice{Value: big.NewInt(0), Digits: "", Indices: nil}, nil
	}
	if k > n {
		return BankChoice{}, fmt.Errorf("cannot select k=%d digits from bank of length %d", k, n)
	}
	toRemove := n - k
	stackDigits := make([]byte, 0, n)
	stackIdx := make([]int, 0, n)

	for i := 0; i < n; i++ {
		d := s[i]
		for len(stackDigits) > 0 && toRemove > 0 && stackDigits[len(stackDigits)-1] < d {
			stackDigits = stackDigits[:len(stackDigits)-1]
			stackIdx = stackIdx[:len(stackIdx)-1]
			toRemove--
		}
		stackDigits = append(stackDigits, d)
		stackIdx = append(stackIdx, i)
	}
	if toRemove > 0 {
		stackDigits = stackDigits[:len(stackDigits)-toRemove]
		stackIdx = stackIdx[:len(stackIdx)-toRemove]
	}
	chosenDigits := stackDigits
	chosenIdx := stackIdx
	if len(chosenDigits) > k {
		chosenDigits = chosenDigits[:k]
		chosenIdx = chosenIdx[:k]
	}
	// build big.Int
	val := big.NewInt(0)
	for _, db := range chosenDigits {
		val.Mul(val, big.NewInt(10))
		val.Add(val, big.NewInt(int64(db-'0')))
	}
	return BankChoice{Value: val, Digits: string(chosenDigits), Indices: chosenIdx}, nil
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
		lineNo++
		line := scanner.Text()
		if strings.TrimSpace(line) == "" {
			continue
		}
		ch, err := bestK(line, K)
		if err != nil {
			log.Fatalf("line %d: %v", lineNo, err)
		}
		choices = append(choices, ch)
	}
	if err := scanner.Err(); err != nil {
		log.Fatalf("scanner error: %v", err)
	}

	total := big.NewInt(0)
	for i, ch := range choices {
		fmt.Printf("Bank %d:  value=%s  digits=%s  indices=%v\n", i+1, ch.Value.String(), ch.Digits, ch.Indices)
		total.Add(total, ch.Value)
	}
	fmt.Println(strings.Repeat("-", 70))
	fmt.Printf("Total joltage (sum of per-bank %d-digit maxima): %s\n", K, total.String())
}
