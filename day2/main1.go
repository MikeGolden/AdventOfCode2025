package main

import (
	"bufio"
	"fmt"
	"log"
	"math"
	"os"
	"strings"
)

func sumInvalidIDs(line string) int64 {
	var total int64 = 0
	ranges := strings.Split(line, ",")

	for _, r := range ranges {
		r = strings.TrimSpace(r)
		if r == "" {
			continue
		}
		var lo, hi int64
		fmt.Sscanf(r, "%d-%d", &lo, &hi)

		maxDigits := int(math.Log10(float64(hi))) + 1

		for n := 1; n <= maxDigits/2; n++ {
			pow10n := int64(math.Pow10(n))

			xMinValid := int64(math.Pow10(n - 1))
			xMaxValid := int64(math.Pow10(n)) - 1

			denom := pow10n + 1

			xMin := (lo + denom - 1) / denom
			xMax := hi / denom

			if xMax < xMinValid || xMin > xMaxValid {
				continue
			}
			if xMin < xMinValid {
				xMin = xMinValid
			}
			if xMax > xMaxValid {
				xMax = xMaxValid
			}
			if xMin > xMax {
				continue
			}

			count := xMax - xMin + 1
			sumX := (xMin + xMax) * count / 2

			total += sumX * denom
		}
	}
	return total
}

func main() {
	file, err := os.Open("input.txt")
	if err != nil {
		log.Fatal(err)
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)
	if !scanner.Scan() {
		log.Fatal("input. txt is empty")
	}

	line := scanner.Text()
	result := sumInvalidIDs(line)
	fmt.Println(result)
}
