#!/usr/bin/env python

from __future__ import annotations
from pathlib import Path
from typing import Iterable
from math import ceil
import sys

def is_primitive_str(s: str) -> bool:
    """Return True if s is not equal to some smaller block repeated."""
    l = len(s)
    for d in range(1, l // 2 + 1):
        if l % d != 0:
            continue
        if s == s[:d] * (l // d):
            return False
    return True

def sum_invalids_from_ranges(rangestr: str) -> int:
    total = 0
    parts = [p.strip() for p in rangestr.split(",") if p.strip()]

    for part in parts:
        lo_s, hi_s = part.split("-")
        lo, hi = int(lo_s), int(hi_s)

        max_digits = len(str(hi))

        # block len n 
        for n in range(1, max_digits + 1):
            # repetition count k >= 2
            max_k = max_digits // n 
            for k in range(2, max_k + 1):
                # geometric sum of 10 ** (n*i)
                big_factor = 0
                pow10n = 10 ** n
                p = 1
                for _ in range(k):
                    big_factor += p
                    p *= pow10n

                # x must not start with 0
                xmin_valid = 10 ** (n - 1)
                xmax_valid = 10 ** n - 1

                xmin = (lo + big_factor - 1) // big_factor
                xmax = hi // big_factor

                if xmax < xmin_valid or xmin > xmax_valid:
                    continue

                xmin = max(xmin, xmin_valid)
                xmax = min(xmax, xmax_valid)
                if xmin > xmax:
                    continue

                for x in range(xmin, xmax + 1):
                    s = str(x)
                    if not is_primitive_str(s):
                        continue
                    total += x * big_factor


    return total


def main() -> None:
    p = Path("input.txt")
    if not p.exists():
        print("input.txt not found", file=sys.stderr)
        raise SystemExit(1)
    line = p.read_text().strip()
    print(sum_invalids_from_ranges(line))

if __name__ == "__main__":
    main()
