from typing import List
import math


def read_input(path: str) -> List[str]:
    with open(path, "r", encoding="utf-8") as f:
        return [line.rstrip("\n") for line in f]


def split_blocks(lines: List[str]) -> List[List[str]]:
    width = max(len(line) for line in lines)
    padded = [line.ljust(width) for line in lines]

    used = [any(row[c] != " " for row in padded) for c in range(width)]

    blocks = []
    start = None

    for i, active in enumerate(used + [False]):
        if active and start is None:
            start = i
        elif not active and start is not None:
            blocks.append([row[start:i] for row in padded])
            start = None

    return blocks


def solve_block(block: List[str]) -> int:
    height = len(block)
    width = len(block[0])

    operator = None
    numbers: List[int] = []

    for col in range(width - 1, -1, -1):
        digits = []
        for row in range(height):
            ch = block[row][col]
            if ch.isdigit():
                digits.append(ch)
            elif ch in "+*":
                operator = ch

        if digits:
            numbers.append(int("".join(digits)))

    if operator == "+":
        return sum(numbers)
    if operator == "*":
        return math.prod(numbers)

    raise ValueError("Operator not found")


def main() -> None:
    lines = read_input("input.txt")
    blocks = split_blocks(lines)
    total = sum(solve_block(b) for b in blocks)
    print(total)


if __name__ == "__main__":
    main()
