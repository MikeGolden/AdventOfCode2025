import * as fs from "fs";

type Range = {
  start: number;
  end: number;
};

function parseRanges(path: string): Range[] {
  const lines = fs.readFileSync(path, "utf-8").split("\n");

  const ranges: Range[] = [];
  for (const line of lines) {
    const trimmed = line.trim();
    if (!trimmed) break;

    const [start, end] = trimmed.split("-").map(Number);
    ranges.push({ start, end });
  }

  return ranges;
}

function countUnionSize(ranges: Range[]): number {
  if (ranges.length === 0) return 0;

  ranges.sort((a, b) => a.start - b.start);

  let total = 0;
  let currentStart = ranges[0].start;
  let currentEnd = ranges[0].end;

  for (const r of ranges.slice(1)) {
    if (r.start > currentEnd + 1) {
      total += currentEnd - currentStart + 1;
      currentStart = r.start;
      currentEnd = r.end;
    } else {
      currentEnd = Math.max(currentEnd, r.end);
    }
  }

  total += currentEnd - currentStart + 1;
  return total;
}

function main(): void {
  const ranges = parseRanges("input.txt");
  console.log(countUnionSize(ranges));
}

main();
