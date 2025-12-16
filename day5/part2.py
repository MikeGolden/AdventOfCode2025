from __future__ import annotations
from dataclasses import dataclass
from typing import List


@dataclass(frozen=True)
class Range:
    start: int
    end: int


def parse_ranges(path: str) -> List[Range]:
    ranges: List[Range] = []

    with open(path, "r", encoding="utf-8") as f:
        for line in f:
            line = line.strip()
            if not line:
                break
            start, end = map(int, line.split("-"))
            ranges.append(Range(start, end))

    return ranges


def count_union_size(ranges: List[Range]) -> int:
    if not ranges:
        return 0

    sorted_ranges = sorted(ranges, key=lambda r: r.start)

    total = 0
    current_start = sorted_ranges[0].start
    current_end = sorted_ranges[0].end

    for r in sorted_ranges[1:]:
        if r.start > current_end + 1:
            total += current_end - current_start + 1
            current_start, current_end = r.start, r.end
        else:
            current_end = max(current_end, r.end)

    total += current_end - current_start + 1
    return total


def main() -> None:
    ranges = parse_ranges("input.txt")
    print(count_union_size(ranges))


if __name__ == "__main__":
    main()
