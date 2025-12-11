import * as fs from "fs";

type Grid = string[][];

/**
 * Load grid from input.txt, ignoring empty lines.
 */
function loadGrid(path: string): Grid {
  const raw = fs.readFileSync(path, "utf8");
  return raw
    .split(/\r?\n/)
    .filter((ln) => ln.trim().length > 0)
    .map((ln) => ln.split(""));
}

/**
 * Find all accessible '@' cells (r,c) with fewer than 4 adjacent '@'.
 */
function findAccessible(grid: Grid): Array<[number, number]> {
  if (grid.length === 0) return [];

  const dirs: Array<[number, number]> = [
    [-1, -1],
    [-1, 0],
    [-1, 1],
    [0, -1],
    [0, 1],
    [1, -1],
    [1, 0],
    [1, 1],
  ];

  const result: Array<[number, number]> = [];

  for (let r = 0; r < grid.length; r++) {
    for (let c = 0; c < grid[r].length; c++) {
      if (grid[r][c] !== "@") continue;
      let adj = 0;
      for (const [dr, dc] of dirs) {
        const nr = r + dr;
        const nc = c + dc;
        if (nr >= 0 && nr < grid.length && nc >= 0 && nc < grid[nr].length) {
          if (grid[nr][nc] === "@") {
            adj++;
            if (adj >= 4) break; // short-circuit
          }
        }
      }
      if (adj < 4) result.push([r, c]);
    }
  }
  return result;
}

/**
 * Repeatedly remove accssible rolls until stable. Return total removed.
 */
function removeUntilStable(grid: Grid): number {
  let total = 0;
  while (true) {
    const toRemove = findAccessible(grid);
    if (toRemove.length === 0) break;
    for (const [r, c] of toRemove) {
      if (grid[r][c] === "@") {
        grid[r][c] = ".";
        total++;
      }
    }
  }
  return total;
}

function main(): void {
  const grid = loadGrid("input.txt");
  console.log(removeUntilStable(grid));
}

main();
