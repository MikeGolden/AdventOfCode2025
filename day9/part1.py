from typing import List, Tuple


def read_points(path: str) -> List[Tuple[int, int]]:
    points: List[Tuple[int, int]] = []
    with open(path, "r", encoding="utf-8") as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            x, y = map(int, line.split(","))
            points.append((x, y))
    return points


def largest_rectangle_area(points: List[Tuple[int, int]]) -> int:
    max_area = 0
    n = len(points)

    for i in range(n):
        x1, y1 = points[i]
        for j in range(i + 1, n):
            x2, y2 = points[j]
            area = (abs(x1 - x2) + 1) * (abs(y1 - y2) + 1)
            if area > max_area:
                max_area = area

    return max_area


def main() -> None:
    points = read_points("input.txt")
    result = largest_rectangle_area(points)
    print(result)


if __name__ == "__main__":
    main()
