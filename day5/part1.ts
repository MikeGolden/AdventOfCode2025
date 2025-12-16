import * as fs from "fs";

type Range = {
  start: number;
  end: number;
};

function contains(range: Range, value: number): boolean {
  return value >= range.start && value <= range.end;
}

function parseInput(path: string): { ranges: Range[]; ids: number[] } {
  const content = fs.readFileSync(path, "utf-8");
  const lines = content.split("\n").map((line) => line.trim());

  const separatorIndex = lines.indexOf("");

  const ranges: Range[] = lines.slice(0, separatorIndex).map((line) => {
    const [start, end] = line.split("-").map(Number);
    return { start, end };
  });

  const ids: number[] = lines
    .slice(separatorIndex + 1)
    .filter(Boolean)
    .map(Number);

  return { ranges, ids };
}

function countFreshIDs(ranges: Range[], ids: number[]): number {
  let count = 0;

  for (const value of ids) {
    if (ranges.some((range) => contains(range, value))) {
      count++;
    }
  }

  return count;
}

function main(): void {
  const { ranges, ids } = parseInput("input.txt");
  const result = countFreshIDs(ranges, ids);
  console.log(result);
}

main();
