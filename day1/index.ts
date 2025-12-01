import * as fs from "fs";

function main() {
  const text = fs.readFileSync("input.txt", "utf8");
  const lines = text.split(/\r?\n/);

  let pos = 50;
  let countZero = 0;

  for (const raw of lines) {
    const line = raw.trim();
    if (!line) continue;

    const dir = line[0];
    const dist = parseInt(line.slice(1), 10);

    if (dir === "R") {
      pos = (pos + dist) % 100;
    } else if (dir === "L") {
      pos = (pos - dist) % 100;
    } else {
      throw new Error(`Bad direction '${dir}' in line '${line}'`);
    }

    if (pos === 0) countZero++;
  }

  console.log(countZero);
}

main();
