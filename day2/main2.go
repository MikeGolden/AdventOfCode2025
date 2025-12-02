package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"
)

func isPrimitive(s string) bool {
	L := len(s)
	for d := 1; d <= L/2; d++ {
		if L%d != 0 {
			continue
		}
		pat := s[:d]
		ok := true
		for i := d; i < L; i += d {
			if s[i:i+d] != pat {
				ok = false
				break
			}
		}
		if ok {
			return false
		}
	}
	return true
}

func sumInvalidsPart2(line string) int64 {
	var total int64 = 0
	parts := strings.Split(line, ",")
	for _, part := range parts {
		part = strings.TrimSpace(part)
		if part == "" {
			continue
		}
		fields := strings.Split(part, "-")
		lo, _ := strconv.ParseInt(fields[0], 10, 64)
		hi, _ := strconv.ParseInt(fields[1], 10, 64)
		maxDigits := len(strconv.FormatInt(hi, 10))
		for n := 1; n <= maxDigits; n++ {
			maxK := maxDigits / n
			for k := 2; k <= maxK; k++ {
				// build bigFactor = 1 + 10^n + 10^(2n) + ...
				var bigFactor int64 = 0
				pow := int64(1)
				pow10n := int64(1)
				for i := 0; i < n; i++ { pow10n *= 10 }
				for i := 0; i < k; i++ {
					bigFactor += pow
					pow *= pow10n
				}

				xminValid := int64(1)
				for i := 0; i < n-1; i++ { xminValid *= 10 }
				xmaxValid := int64(1)
				for i := 0; i < n; i++ { xmaxValid *= 10 }
				xmaxValid -= 1

				xmin := (lo + bigFactor - 1) / bigFactor
				xmax := hi / bigFactor
				if xmax < xminValid || xmin > xmaxValid {
					continue
				}
				if xmin < xminValid { xmin = xminValid }
				if xmax > xmaxValid { xmax = xmaxValid }
				for X := xmin; X <= xmax; X++ {
					s := strconv.FormatInt(X, 10)
					if !isPrimitive(s) {
						continue
					}
					total += X * bigFactor
				}
			}
		}
	}
	return total
}

func main() {
	f, err := os.Open("input.txt")
	if err != nil {
		log.Fatal(err)
	}
	defer f.Close()
	sc := bufio.NewScanner(f)
	if !sc.Scan() {
		log.Fatal("input.txt empty")
	}
	line := sc.Text()
	fmt.Println(sumInvalidsPart2(line))
}