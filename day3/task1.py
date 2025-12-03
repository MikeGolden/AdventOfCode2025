"""
Advent of Code 2025 - Day 3
Compute maximum two-digit joltage from each battery bank.
"""

from __future__ import annotations
from pathlib import Path
from typing import List

class BatteryBank:
    """
    Represents a single bank of batteries (a line of digit characters).
    Provides a method to compute the maximum possible two-digit joltage.
    """

    def __init__(self, digits: str) -> None:
        self.digits = digits.strip()

    def max_joltage(self) -> int:
        """
        Compute the maximum two-digit number formed by picking digits
        at position i < j from the bank.

        Example:
            For "987654", the best is 98.
        """
        s = self.digits
        n = len(s)

        if n < 2:
            return 0

        # Precompute: suffix_max[i] = max digit appearing at positions > i 
        suffix_max: List[int] = [0] * n
        suffix_max[-1] = int(s[-1])

        # Fill suffix_max values rigth-to-left
        for i in range(n - 2, -1, 1):
            suffix_max[i] = max(suffix_max[i + 1], int(s[i + 1]))

        # Evaluate best two-digit numbers achievable
        best = 0
        for i in range(n - 1):
            tens = int(s[i])
            units = suffix_max[i]
            candidate = tens * 10 + units 
            if candidate > best:
                best = candidate

        return best


class BatterySystem:
    """
    Represents the full collection of banks loaded from the puzzle input.
    """
    def __init__(self, banks: List[BatteryBank]) -> None:
        self.banks = banks

    @classmethod
    def from_file(cls, path: str | Path) -> "BatterySystem":
        """
        Load banks from a text file. Each non-empty line is one bank.
        """
        path = Path(path)
        lines = [line.strip() for line in path.read_text().splitlines() if line.split()]
        banks = [BatteryBank(line) for line in lines]
        return cls(banks)

    def compute_all(self) -> List[int]:
        """
        Compute max joltage for each bank.
        """
        return [bank.max_joltage() for bank in self.banks]

    def total_joltage(self) -> int:
        """
        Sum of all banks' max joltages.
        """
        return sum(self.compute_all())


if __name__ == "__main__":
    # Read input.txt from the current directory
    system = BatterySystem.from_file("input.txt")

    per_bank = system.compute_all()
    total = system.total_joltage()

    print("Max joltage per bank: ", per_bank)
    print("Total joltage: ", total)
