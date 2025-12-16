import * as fs from "fs";

function readInput(path: string): string[] {
  return fs
    .readFileSync(path, "utf-8")
    .split("\n")
    .map((line) => line.trim())
    .filter((line) => line.length > 0);
}

function countSplits(grid: string[]): number {
  const rows = grid.length;
  const cols = grid[0].length;

  const startCol = grid[0].indexOf("S");
  let active = new Set<number>([startCol]);
  let splits = 0;

  for (let r = 1; r < rows; r++) {
    const next = new Set<number>();

    for (const c of active) {
      if (c < 0 || c >= cols) continue;

      if (grid[r][c] === "^") {
        splits++;
        if (c - 1 >= 0) next.add(c - 1);
        if (c + 1 < cols) next.add(c + 1);
      } else {
        next.add(c);
      }
    }

    if (next.size === 0) break;
    active = next;
  }

  return splits;
}

const grid = readInput("input.txt");
console.log(countSplits(grid));
