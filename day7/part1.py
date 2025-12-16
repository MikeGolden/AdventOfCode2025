from typing import List, Set


def read_input(path: str) -> List[str]:
    with open(path, "r", encoding="utf-8") as f:
        return [line.rstrip("\n") for line in f if line.strip()]


def count_splits(grid: List[str]) -> int:
    rows = len(grid)
    cols = len(grid[0])

    # Find start column
    start_col = grid[0].index("S")

    active: Set[int] = {start_col}
    splits = 0

    for r in range(1, rows):
        next_active: Set[int] = set()

        for c in active:
            if c < 0 or c >= cols:
                continue

            cell = grid[r][c]
            if cell == "^":
                splits += 1
                if c - 1 >= 0:
                    next_active.add(c - 1)
                if c + 1 < cols:
                    next_active.add(c + 1)
            else:
                next_active.add(c)

        active = next_active

        if not active:
            break

    return splits


if __name__ == "__main__":
    grid = read_input("input.txt")
    print(count_splits(grid))
