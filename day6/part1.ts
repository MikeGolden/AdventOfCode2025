import * as fs from "fs";

function readInput(path: string): string[] {
  return fs.readFileSync(path, "utf8").split("\n");
}

function splitBlocks(lines: string[]): string[][] {
  const width = Math.max(...lines.map((l) => l.length));
  const padded = lines.map((l) => l.padEnd(width, " "));

  const used = Array(width).fill(false);
  for (let c = 0; c < width; c++) {
    for (const row of padded) {
      if (row[c] !== " ") {
        used[c] = true;
        break;
      }
    }
  }

  const blocks: string[][] = [];
  let start: number | null = null;

  for (let i = 0; i <= width; i++) {
    const active = i < width && used[i];
    if (active && start === null) {
      start = i;
    } else if (!active && start !== null) {
      blocks.push(padded.map((r) => r.slice(start!, i)));
      start = null;
    }
  }

  return blocks;
}

function solveBlock(block: string[]): bigint {
  const numbers: bigint[] = [];
  let op: string | null = null;

  for (const row of block) {
    const s = row.trim();
    if (!s) continue;
    if (s === "+" || s === "*") {
      op = s;
    } else {
      numbers.push(BigInt(s));
    }
  }

  if (op === "+") {
    return numbers.reduce((a, b) => a + b, 0n);
  }
  return numbers.reduce((a, b) => a * b, 1n);
}

const lines = readInput("input.txt");
const blocks = splitBlocks(lines);
const total = blocks.reduce((sum, b) => sum + solveBlock(b), 0n);

console.log(total.toString());
