#!/usr/bin/env python3
"""
Advent of Code 2025 — Day 3, "Lobby"
For each line in input.txt (a bank of single-digit batteries), choose exactly two batteries
at positions i < j to form the two-digit number 10*d[i] + d[j]. Output the best choice
for each bank (value, digits, indices) and the overall total.
"""

from __future__ import annotations
from dataclasses import dataclass
from pathlib import Path
from typing import List, Optional, Tuple


@dataclass(frozen=True)
class Choice:
    """Represents a chosen pair of batteries for a bank."""
    value: int              # two-digit number 10*tens + units
    tens_digit: int         # digit used as tens (d[i])
    units_digit: int        # digit used as units (d[j])
    tens_index: int         # index i (0-based) of tens digit
    units_index: int        # index j (0-based) of units digit


class BatteryBank:
    """Encapsulates a single bank (one input line) and logic to pick best two batteries."""

    def __init__(self, digits_line: str) -> None:
        """
        digits_line: string containing digits (whitespace trimmed). Non-digit characters are invalid.
        """
        s = digits_line.strip()
        if not s:
            raise ValueError("Empty battery bank line provided.")
        if not all(ch.isdigit() for ch in s):
            raise ValueError(f"Bank contains non-digit characters: {digits_line!r}")
        # Convert chars to ints (0-9). Problem states 1-9; we accept 0-9 defensively.
        self.digits: List[int] = [int(ch) for ch in s]

    def best_choice(self) -> Choice:
        """
        Find the best two-battery choice (i < j) maximizing 10*d[i] + d[j].
        Returns a Choice with value, digits, and indices.

        Algorithm: single pass from right to left tracking the maximum digit to the right and its
        (rightmost) index. For each left position i, candidate = 10*d[i] + max_digit_right.
        Complexity: O(n).
        """
        n = len(self.digits)
        if n < 2:
            # No valid pair; define behavior: produce value 0 and indices -1
            return Choice(value=0, tens_digit=0, units_digit=0, tens_index=-1, units_index=-1)

        # suffix_max_digit[i] = maximum digit among positions > i
        # but we can compress to two scalars while scanning left-to-right after precomputing suffix info.
        suffix_max_digit: List[int] = [0] * n
        suffix_max_index: List[int] = [-1] * n

        # initialize with the last digit (position n-1) as the rightmost element;
        # for i = n-2 we want max among positions > i which includes position n-1.
        suffix_max_digit[-1] = self.digits[-1]
        suffix_max_index[-1] = n - 1

        for i in range(n - 2, -1, -1):
            right_digit = self.digits[i + 1]
            # compare the immediate right digit vs. the previously computed suffix max
            if right_digit >= suffix_max_digit[i + 1]:
                # prefer the rightmost occurrence of the max digit (>= ensures rightmost wins)
                suffix_max_digit[i] = right_digit
                suffix_max_index[i] = i + 1
            else:
                suffix_max_digit[i] = suffix_max_digit[i + 1]
                suffix_max_index[i] = suffix_max_index[i + 1]

        best_value = -1
        best_tens = 0
        best_units = 0
        best_i = -1
        best_j = -1

        for i in range(n - 1):
            tens = self.digits[i]
            units = suffix_max_digit[i]
            units_idx = suffix_max_index[i]
            candidate = tens * 10 + units
            # choose strictly larger candidate, ties unlikely but kept deterministic by earlier rightmost selection
            if candidate > best_value:
                best_value = candidate
                best_tens = tens
                best_units = units
                best_i = i
                best_j = units_idx

        # safety check
        if best_i < 0 or best_j <= best_i:
            # this should not happen, but guard for logic errors
            raise RuntimeError("Failed to find a valid pair in non-empty bank.")

        return Choice(
            value=best_value,
            tens_digit=best_tens,
            units_digit=best_units,
            tens_index=best_i,
            units_index=best_j,
        )


class BatterySystem:
    """Represents the whole input (multiple banks)."""

    def __init__(self, banks: List[BatteryBank]) -> None:
        self.banks = banks

    @classmethod
    def from_file(cls, path: str | Path) -> "BatterySystem":
        p = Path(path)
        if not p.exists():
            raise FileNotFoundError(f"Input file not found: {p!s}")
        raw = p.read_text(encoding="utf-8")
        lines = [ln for ln in raw.splitlines() if ln.strip()]
        banks = [BatteryBank(ln) for ln in lines]
        return cls(banks)

    def evaluate(self) -> List[Choice]:
        """Return the best Choice for each bank in order."""
        return [bank.best_choice() for bank in self.banks]

    def total_joltage(self) -> int:
        return sum(choice.value for choice in self.evaluate())


def _format_choice(idx: int, ch: Choice) -> str:
    """Helper for pretty-printing a bank's chosen pair."""
    if ch.tens_index < 0:
        return f"Bank {idx}: EMPTY or too short -> value=0"
    return (
        f"Bank {idx}: value={ch.value:02d}  "
        f"(tens digit={ch.tens_digit} at idx={ch.tens_index}, "
        f"units digit={ch.units_digit} at idx={ch.units_index})"
    )


def main(input_path: str = "input.txt") -> int:
    system = BatterySystem.from_file(input_path)
    choices = system.evaluate()
    total = system.total_joltage()

    for idx, ch in enumerate(choices, start=1):
        print(_format_choice(idx, ch))
    print("-" * 60)
    print(f"Total joltage: {total}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main("input.txt"))