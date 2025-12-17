import * as fs from "fs";

type Key = string;

function readInput(path: string): string[] {
  return fs
    .readFileSync(path, "utf8")
    .split("\n")
    .map((l) => l.trim())
    .filter((l) => l.length > 0);
}

function key(r: number, c: number): Key {
  return `${r},${c}`;
}

function countTimeLines(grid: string[]): bigint {
  const rows = grid.length;
  const cols = grid[0].length;
  const startCol = grid[0].indexOf("S");

  let dp = new Map<Key, bigint>();
  dp.set(key(0, startCol), 1n);

  let total = 0n;

  for (let r = 0; r < rows; r++) {
    const next = new Map<Key, bigint>();

    for (const [k, count] of dp) {
      const [row, col] = k.split(",").map(Number);

      if (row !== r) continue;

      if (col < 0 || col >= cols || row === rows - 1) {
        total += count;
        continue;
      }

      if (grid[row][col] === "^") {
        next.set(
          key(row + 1, col - 1),
          (next.get(key(row + 1, col - 1)) ?? 0n) + count,
        );
        next.set(
          key(row + 1, col + 1),
          (next.get(key(row + 1, col + 1)) ?? 0n) + count,
        );
      } else {
        next.set(
          key(row + 1, col),
          (next.get(key(row + 1, col)) ?? 0n) + count,
        );
      }
    }

    dp = next;
  }

  return total;
}

const grid = readInput("input.txt");
console.log(countTimeLines(grid).toString());
