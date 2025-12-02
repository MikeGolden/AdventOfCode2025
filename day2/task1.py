#!/usr/bin/env python

"""
Advent of Code 2025 - Day 2
Detect IDs that consist of a block of digits repeated twice (e.g. 55, 123123)
"""

from __future__ import annotations
from os import initgroups
from pathlib import Path


def sum_invalid_ids(range_line: str) -> int:
    """
    Given a single-line comma-separated list of ranges(e.g. "11-22,95-115,..."),
    compute the sum of all numbers in those ranges that have the form XX
    where X is a block of digits with no leading zeros
    """

    total = 0
    ranges = [r.strip() for r in range_line.split(",") if r.strip()]

    for r in ranges:
        lo_s, hi_s = r.split("-")
        lo, hi = int(lo_s), int(hi_s)

        # Max digits determine maximal possible repeated-block length.
        max_digits = len(str(hi))

        # Only even lengths possible -> block size n means final number has 2n digits.
        for n in range(1, max_digits // 2 + 1):
            pow10n = 10 ** n

            # Block must be n digits, cannot start with zero → X ∈ [10^(n-1), 10^n − 1]
            x_min_valid = 10 ** (n - 1)
            x_max_valid = 10 ** n - 1

            # Number is: X * (10^n + 1)
            denom = pow10n + 1 

            # Solve lo ≤ X*denom ≤ hi
            x_min = (lo + denom - 1) // denom   # ceil(lo / denom)
            x_max = hi // denom                 # floor(hi / denom)

            # Restrict to valid block range
            if x_max < x_min_valid or x_min > x_max_valid:
                continue

            x_min = max(x_min, x_min_valid)
            x_max = min(x_max, x_max_valid)

            if x_min > x_max:
                continue

            # Sum of X*denom for X in [x_min, x_max]
            count = x_max - x_min + 1 
            block_sum = (x_min + x_max) * count // 2
            total += block_sum * denom

    return total

def main() -> None:
    input_path = Path("input.txt")
    if not input_path.exists():
        raise FileNotFoundError("input.txt not found")

    range_line = input_path.read_text().strip()
    result = sum_invalid_ids(range_line)
    print(result)

if __name__ == "__main__":
    main()
