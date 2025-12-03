#!/usr/bin/env python
"""
Advent of Code 2025 — Day 3 Part Two (Lobby)

Select exactly k digits (in order) from each line (bank) of the input file to form the
largest possible k-digit number for that bank. Sum the resulting numbers across all banks.

Algorithm:
  - To select k digits from n, use a greedy monotonic stack approach (O(n) per line):
    iterate digits left-to-right, and keep a stack that we are allowed to pop while
    doing so increases the lexicographic value and we still have digits to drop
    (n - k in total). This produces the lexicographically largest subsequence
    of length k while preserving original order.
"""

from __future__ import annotations
from dataclasses import dataclass
from pathlib import Path
from typing import List, Tuple
import argparse
import sys


@dataclass(frozen=True)
class Choice:
    """Represents the chosen subsequence for a bank."""
    value: int            # integer value of the chosen k-digit number
    digits: List[int]     # list of selected digits (length k)
    indices: List[int]    # corresponding indices in the original line(0-based)

class BatteryBank:
    """Encapsulates a bank of battery digits and logic to pick the best k-digit subsequence."""

    def __init__(self, line: str) -> None:
        s = line.strip()
        if not s:
            raise ValueError("Empty bank line encountered")
        if not all(ch.isdigit() for ch in s):
            raise ValueError(f"Bank contains non-digit characters: {line!r}")
        # Convert characters to integers (0-9). Problem states 1-9 but accept 0-9 defensively.
        self.digits: List[int] = [int(ch) for ch in s]

    def best_choice(self, k: int) -> Choice:
        """
        Return the best Choice selecting exactly k digits (in order) to maximize the k-digit number.
        Uses a monotonic stack (greedy) algorithm in O(n) time and O(n) extra memory.
        """
        n = len(self.digits)
        if k < 0:
            raise ValueError("k must be non-negative")
        if k == 0:
            return Choice(value=0, digits=[], indices=[])
        if k > n:
            raise ValueError(f"Cannot select k={k} digits from bank of length {n}")

        to_remove = n - k # how many digits we must drop
        stack_digits: List[int] = []
        stack_indices: List[int] = []

        for i, d in enumerate(self.digits):
            # While we can remove previous smaller digits to make room for bigger current digit,
            # and we still need to remove digits overall, pop from stack.
            while stack_digits and to_remove > 0 and stack_digits[-1] < d:
                stack_digits.pop()
                stack_indices.pop()
                to_remove -= 1
            stack_digits.append(d)
            stack_indices.append(i)

        # If we still need to remove digits (stack is non-decreasing), remove from the end
        if to_remove > 0:
            stack_digits = stack_digits[:-to_remove]
            stack_indices = stack_indices[:-to_remove]

        # The stack now contains at least k elements; take the first k (they are the selected subsequence)
        chosen_digits = stack_digits[:k]
        chosen_indices = stack_indices[:k]

        # Build integer value safely (works for arbitrarily large k)
        value = 0
        for dig in chosen_digits:
            value = value * 10 + dig

        return Choice(value=value, digits=chosen_digits, indices=chosen_indices)

class BatterySystem:
    """Holds multiple BatteryBank objects and computes per-bank results and totals."""

    def __init__(self, banks: List[BatteryBank]) -> None:
        self.banks = banks

    @classmethod
    def from_file(cls, path: str | Path) -> BatterySystem:
        p = Path(path)
        if not p.exists():
            raise FileNotFoundError(f"Input file not found: {p}")
        raw = p.read_text(encoding="utf-8")
        lines = [ln for ln in raw.splitlines() if ln.strip()]
        banks = [BatteryBank(ln) for ln in lines]
        return cls(banks)

    def evaluate_all(self, k: int) -> List[Choice]:
        return [bank.best_choice(k) for bank in self.banks]

    def total_joltage(self, k: int) -> int:
        return sum(choice.value for choice in self.evaluate_all(k))


def format_choice(idx: int, choice: Choice) -> str:
    """Pretty-print one bank's result (1-based index)."""
    digits_str = "".join(str(d) for d in choice.digits)
    indices_str = ", ".join(str(i) for i in choice.indices)
    return f"Bank {idx}: value={choice.value}  digits={digits_str}  indices=[{indices_str}]"

def parse_args(argv: List[str]) -> argparse.Namespace:
    p = argparse.ArgumentParser(description="Advent of Code 2025 - Day 3 Part Two")
    p.add_argument("--input", "-i", default="input.txt", help="Input file path (default: input.txt)")
    p.add_argument("--k", "-k", type=int, default=12, help="Number of digits to select per bank (default: 12)")
    return p.parse_args(argv)

def main(argv: List[str] | None = None) -> int:
    args = parse_args(sys.argv[1:] if argv is None else argv)
    try:
        system = BatterySystem.from_file(args.input)
    except Exception as e:
        print(f"Error reading input: {e}", file=sys.stderr)
        return 2

    try:
        choices = system.evaluate_all(args.k)
    except Exception as e:
        print(f"Error computing choices: {e}", file=sys.stderr)
        return 3

    total = sum(c.value for c in choices)

    for idx, ch in enumerate(choices, start=1):
        print(format_choice(idx, ch))
    print("-" * 72)
    print(f"Total joltage (sum of per-bank {args.k}-digit maxima: {total}")
    return 0

if __name__ == "__main__":
    raise SystemExit(main())

