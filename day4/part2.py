#!/usr/bin/env python

from __future__ import annotations
from typing import List, Tuple


def load_grid(path: str) -> List[List[str]]:
    """
    Load non-empty lines from `path` into 2D list of characters.
    """
    grid: List[List[str]] = []
    with open(path, "r", encoding="utf-8") as f:
        for raw in f:
            line = raw.strip("\n")
            if line.strip():
                grid.append(list(line))
    return grid


def find_accessible(grid: List[List[str]]) -> List[Tuple[int, int]]:
    """
    Return a list of coordinates (r, c) of '@' cells that have fewer
    than 4 adjacent '@' neighbors.
    """
    if not grid:
        return []

    h = len(grid)
    dirs: List[Tuple[int, int]] = [
        (-1, -1),
        (-1, 0),
        (-1, 1),
        (0, -1),
        (0, 1),
        (1, -1),
        (1, 0),
        (1, 1),
    ]

    accessible: List[Tuple[int, int]] = []

    for r in range(h):
        row_len = len(grid[r])
        for c in range(row_len):
            if grid[r][c] != "@":
                continue
            adj = 0
            for dr, dc in dirs:
                nr, nc = r + dr, c + dc
                if 0 <= nr < h and 0 <= nc < len(grid[nr]) and grid[nr][nc] == "@":
                    adj += 1
                    if adj >= 4:
                        break
            if adj < 4:
                accessible.append((r, c))

    return accessible


def remove_until_stable(grid: List[List[str]]) -> int:
    """
    Repeatedly remove all accessible '@' (set to '.') until no more can be removed.
    Return total removed count.
    """
    total_removed = 0
    while True:
        to_remove = find_accessible(grid)
        if not to_remove:
            break
        for r, c in to_remove:
            if grid[r][c] == "@":
                grid[r][c] = "."
                total_removed += 1
    return total_removed


def main() -> None:
    grid = load_grid("input.txt")
    removed = remove_until_stable(grid)
    print(removed)


if __name__ == "__main__":
    main()
