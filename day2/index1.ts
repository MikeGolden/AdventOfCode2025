import * as fs from "fs";

function sumInvalidIDs(line: string): number {
  let total = 0;
  const parts = line
    .split(",")
    .map((s) => s.trim())
    .filter(Boolean);

  for (const part of parts) {
    const [loStr, hiStr] = part.split("-");
    const lo = parseInt(loStr, 10);
    const hi = parseInt(hiStr, 10);

    const maxDigits = hi.toString().length;

    for (let n = 1; n <= Math.floor(maxDigits / 2); n++) {
      const pow10n = 10 ** n;

      const xMinValid = 10 ** (n - 1);
      const xMaxValid = 10 ** n - 1;

      const denom = pow10n + 1;

      let xMin = Math.ceil(lo / denom);
      let xMax = Math.floor(hi / denom);

      if (xMax < xMinValid || xMin > xMaxValid) continue;

      xMin = Math.max(xMin, xMinValid);
      xMax = Math.min(xMax, xMaxValid);

      if (xMin > xMax) continue;

      const count = xMax - xMin + 1;
      const sumX = ((xMin + xMax) * count) / 2;

      total += sumX * denom;
    }
  }

  return total;
}

function main() {
  const data = fs.readFileSync("input.txt", "utf8").trim();
  const result = sumInvalidIDs(data);
  console.log(result);
}

main();
