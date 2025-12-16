#!/usr/bin/env python
from __future__ import annotations

from dataclasses import dataclass
from typing import List, Tuple
from warnings import resetwarnings


@dataclass(frozen=True)
class Range:
    start: int
    end: int

    def contains(self, value: int) -> bool:
        return self.start <= value <= self.end


def parse_input(path: str) -> tuple[List[Range], list[int]]:
    ranges: List[Range] = []
    ids: List[int] = []

    with open(path, "r", encoding="utf-8") as f:
        lines = [line.strip() for line in f]

    separator_index = lines.index("")

    for line in lines[:separator_index]:
        start, end = map(int, line.split("-"))
        ranges.append(Range(start, end))

    for line in lines[separator_index + 1 :]:
        if line:
            ids.append(int(line))

    return ranges, ids


def count_fresh_ids(ranges: List[Range], ids: List[int]) -> int:
    count = 0
    for value in ids:
        if any(r.contains(value) for r in ranges):
            count += 1
    return count


def main() -> None:
    ranges, ids = parse_input("input.txt")
    result = count_fresh_ids(ranges, ids)
    print(result)


if __name__ == "__main__":
    main()
