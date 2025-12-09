from __future__ import annotations
from typing import List, Tuple


def load_grid(path: str) -> List[List[str]]:
    """
    Load the puzzle input from a text file.

    Each non-empty line becomes a row of characters.
    Empty of whitespace-only lines are ignored.
    """
    grid: List[List[str]] = []

    with open(path, "r", encoding="utf-8") as f:
        for raw_line in f:
            line = raw_line.rstrip("\n")
            if line.strip():
                grid.append(list(line))

    return grid


def count_accessible_rolls(grid: List[List[str]]) -> int:
    """
    Count the number of '@' rolls that have fewer than 4 adjacent '@'
    neighbors among the 8 surrounding cells.

    Args:
        grid: 2D list representing the ASCII map.
    Returns:
        Number of accessible rolls.
    """
    if not grid:
        return 0

    h: int = len(grid)

    # All 8 directions around a given cell
    directions: List[Tuple[int, int]] = [
        (-1, -1),
        (-1, 0),
        (-1, 1),
        (0, -1),
        (0, 1),
        (1, -1),
        (1, 0),
        (1, 1),
    ]

    accessible = 0

    for r in range(h):
        row_len = len(grid[r])
        for c in range(row_len):
            if grid[r][c] != "@":
                continue

            adjacent = 0
            for dr, dc in directions:
                nr = r + dr
                nc = c + dc

                if 0 <= nr < h and 0 <= nc < len(grid[nr]):
                    if grid[nr][nc] == "@":
                        adjacent += 1

            if adjacent < 4:
                accessible += 1

    return accessible


def main() -> None:
    grid = load_grid("input.txt")
    result = count_accessible_rolls(grid)
    print(result)


if __name__ == "__main__":
    main()
