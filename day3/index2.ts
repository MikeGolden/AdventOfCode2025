import * as fs from "fs";

const K = 12;

interface Choice {
  value: bigint;
  digits: string;
  indices: number[];
}

function bestK(line: string, k: number): Choice {
  const s = line.trim();
  if (s.length === 0) return { value: 0n, digits: "", indices: [] };
  const n = s.length;
  for (const ch of s) {
    if (ch < "0" || ch > "9") throw new Error(`Non-digit character: ${ch}`);
  }
  if (k > n) throw new Error(`k=${k} exceeds bank length ${n}`);
  let toRemove = n - k;
  const stackDigits: string[] = [];
  const stackIdx: number[] = [];
  for (let i = 0; i < n; i++) {
    const d = s[i];
    while (
      stackDigits.length > 0 &&
      toRemove > 0 &&
      stackDigits[stackDigits.length - 1] < d
    ) {
      stackDigits.pop();
      stackIdx.pop();
      toRemove--;
    }
    stackDigits.push(d);
    stackIdx.push(i);
  }
  if (toRemove > 0) {
    stackDigits.splice(stackDigits.length - toRemove, toRemove);
    stackIdx.splice(stackIdx.length - toRemove, toRemove);
  }
  const chosenDigits = stackDigits.slice(0, k);
  const chosenIdx = stackIdx.slice(0, k);
  let value = 0n;
  for (const ch of chosenDigits) {
    value = value * 10n + BigInt(ch.charCodeAt(0) - 48);
  }
  return { value, digits: chosenDigits.join(""), indices: chosenIdx };
}

function main() {
  const raw = fs.readFileSync("input.txt", "utf8");
  const lines = raw.split(/\r?\n/).filter((ln) => ln.trim().length > 0);
  const choices = lines.map((ln) => bestK(ln, K));
  let total = 0n;
  choices.forEach((c, i) => {
    console.log(
      `Bank ${i + 1}: value=${c.value.toString()}  digits=${c.digits}  indices=[${c.indices.join(", ")}]`,
    );
    total += c.value;
  });
  console.log("-".repeat(72));
  console.log(
    `Total joltage (sum of per-bank ${K}-digit maxima): ${total.toString()}`,
  );
}

if (require.main === module) {
  main();
}

