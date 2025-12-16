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
    if (active && start === null) start = i;
    else if (!active && start !== null) {
      blocks.push(padded.map((r) => r.slice(start!, i)));
      start = null;
    }
  }

  return blocks;
}

function solveBlock(block: string[]): bigint {
  const h = block.length;
  const w = block[0].length;

  const numbers: bigint[] = [];
  let op: string | null = null;

  for (let c = w - 1; c >= 0; c--) {
    let digits = "";
    for (let r = 0; r < h; r++) {
      const ch = block[r][c];
      if (/\d/.test(ch)) digits += ch;
      else if (ch === "+" || ch === "*") op = ch;
    }
    if (digits) numbers.push(BigInt(digits));
  }

  return op === "+"
    ? numbers.reduce((a, b) => a + b, 0n)
    : numbers.reduce((a, b) => a * b, 1n);
}

const lines = readInput("input.txt");
const blocks = splitBlocks(lines);
const total = blocks.reduce((s, b) => s + solveBlock(b), 0n);
console.log(total.toString());
