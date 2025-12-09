import * as fs from "fs";

interface BankChoice {
  value: number;
  digits: string;
  tensIndex: number;
  unitsIndex: number;
}

function bestTwo(line: string): BankChoice {
  const s = line.trim();
  if (s.length === 0) {
    return { value: 0, digits: "", tensIndex: -1, unitsIndex: -1 };
  }
  const n = s.length;
  for (const ch of s) {
    if (ch < "0" || ch > "9") throw new Error(`Non-digit character: ${ch}`);
  }

  // suffix max digit ant its rightmost index
  const suffixMax: string[] = new Array(n);
  const suffixIdx: number[] = new Array(n);
  suffixMax[n - 1] = s[n - 1];
  suffixIdx[n - 1] = n - 1;
  for (let i = n - 2; i >= 0; i--) {
    const right = s[i + 1];
    if (right >= suffixMax[i + 1]) {
      suffixMax[i] = right;
      suffixIdx[i] = i + 1;
    } else {
      suffixMax[i] = suffixMax[i + 1];
      suffixIdx[i] = suffixIdx[i + 1];
    }
  }
  let bestVal = -1;
  let bestI = -1;
  let bestJ = -1;
  for (let i = 0; i < n - 1; i++) {
    const tens = s.charCodeAt(i) - 48;
    const units = suffixMax[i].charCodeAt(0) - 48;
    const val = tens * 10 + units;
    if (val > bestVal) {
      bestVal = val;
      bestI = i;
      bestJ = suffixIdx[i];
    }
  }
  if (bestI < 0) return { value: 0, digits: "", tensIndex: -1, unitsIndex: -1 };
  return {
    value: bestVal,
    digits: s[bestI] + s[bestJ],
    tensIndex: bestI,
    unitsIndex: bestJ,
  };
}

function main() {
  const raw = fs.readFileSync("input.txt", "utf8");
  const lines = raw.split(/\r?\n/).filter((ln) => ln.trim().length > 0);
  const choices: BankChoice[] = lines.map(bestTwo);
  let total = 0;
  choices.forEach((c, i) => {
    console.log(
      `Bank ${i + 1}: value=${c.value.toString().padStart(2, "0")} digits=${c.digits} indices=(${c.tensIndex},${c.unitsIndex})`,
    );
    total += c.value;
  });
  console.log("-".repeat(80));
  console.log(`Total joltage: ${total}`);
}

if (require.main === module) {
  main();
}
