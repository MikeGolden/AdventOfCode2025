import * as fs from "fs";

interface Point {
  x: number;
  y: number;
}

function readPoints(path: string): Point[] {
  return fs
    .readFileSync(path, "utf-8")
    .trim()
    .split("\n")
    .map((line) => {
      const [x, y] = line.split(",").map(Number);
      return { x, y };
    });
}

function largestRectangleArea(points: Point[]): number {
  let maxArea = 0;

  for (let i = 0; i < points.length; i++) {
    for (let j = i + 1; j < points.length; j++) {
      const area =
        (Math.abs(points[i].x - points[j].x) + 1) *
        (Math.abs(points[i].y - points[j].y) + 1);
      if (area > maxArea) {
        maxArea = area;
      }
    }
  }

  return maxArea;
}

function main(): void {
  const points = readPoints("input.txt");
  console.log(largestRectangleArea(points));
}

main();
