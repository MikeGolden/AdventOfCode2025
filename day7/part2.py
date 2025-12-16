from typing import List
from collections import defaultdict


def read_input(path: str) -> List[str]:
    with open(path, "r", encoding="utf-8") as f:
        return [line.rstrip("\n") for line in f if line.strip()]


def count_timelines(grid: List[str]) -> int:
    rows, cols = len(grid), len(grid[0])
    start_col = grid[0].index("S")

    dp = defaultdict(int)
    dp[(0, start_col)] = 1

    total = 0

    for r in range(rows):
        next_dp = defaultdict(int)

        for (row, col), count in dp.items():
            if row != r:
                continue

            if col < 0 or col >= cols:
                total += count
                continue

            cell = grid[row][col]

            if row == rows - 1:
                total += count
                continue

            if cell == "^":
                next_dp[(row + 1, col - 1)] += count
                next_dp[(row + 1, col + 1)] += count
            else:
                next_dp[(row + 1, col)] += count

        dp = next_dp

    return total


if __name__ == "__main__":
    grid = read_input("input.txt")
    print(count_timelines(grid))
