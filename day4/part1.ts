import * as fs from 'fs';
import * as path from 'path';

/**
 * Load the puzzle input from a text file.
 *
 * Each non-empty line becomes a row of characters.
 * Empty or whitespace-only lines are ignored.
 *
 * @param filePath The path to the input file.
 * @returns A 2D array of strings representing the grid.
 */
function loadGrid(filePath: string): string[][] {
    const fileContent = fs.readFileSync(filePath, 'utf-8');
    const lines = fileContent.split(/\r?\n/);
    const grid: string[][] = [];

    for (const line of lines) {
        if (line.trim()) {
            grid.push(Array.from(line));
        }
    }

    return grid;
}

/**
 * Count the number of '@' rolls that have fewer than 4 adjacent '@'
 * neighbors among the 8 surrounding cells.
 *
 * @param grid 2D list representing the ASCII map.
 * @returns Number of accessible rolls.
 */
function countAccessibleRolls(grid: string[][]): number {
    if (!grid.length) {
        return 0;
    }

    const h: number = grid.length;

    // All 8 directions around a given cell
    const directions: [number, number][] = [
        [-1, -1], [-1, 0], [-1, 1],
        [0, -1],           [0, 1],
        [1, -1], [1, 0], [1, 1],
    ];

    let accessible = 0;

    for (let r = 0; r < h; r++) {
        const rowLen = grid[r].length;
        for (let c = 0; c < rowLen; c++) {
            if (grid[r][c] !== '@') {
                continue;
            }

            let adjacent = 0;
            for (const [dr, dc] of directions) {
                const nr = r + dr;
                const nc = c + dc;

                if (nr >= 0 && nr < h && nc >= 0 && nc < grid[nr].length) {
                    if (grid[nr][nc] === '@') {
                        adjacent++;
                    }
                }
            }

            if (adjacent < 4) {
                accessible++;
            }
        }
    }

    return accessible;
}

/**
 * Main function to run the script.
 */
function main(): void {
    const grid = loadGrid(path.join(__dirname, 'input.txt'));
    const result = countAccessibleRolls(grid);
    console.log(result);
}

main();
