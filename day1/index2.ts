import * as fs from "fs";

function zerosDuring(pos: number, dist: number, direction: number): number {
    let firstStep: number;
    if (direction === 1) {
        firstStep = (100 - pos) % 100;
    } else {
        firstStep = pos % 100;
    }

    if (firstStep === 0) {
        if (dist < 100) return 0;
        return 1 + Math.floor((dist - 100) / 100);
    }
    if (dist < firstStep) return 0;
    return 1 + Math.floor((dist - firstStep) / 100);
}

function main() {
    const text = fs.readFileSync("input.txt", "utf8");
    const lines = text.split(/\r?\n/).map(s => s.trim()).filter(Boolean);

    let pos = 50;
    let countZero = 0;

    for (const line of lines) {
        const dir = line[0];
        const dist = parseInt(line.slice(1), 10);
        const direction = dir === "R" ? 1 : -1;

        countZero += zerosDuring(pos, dist, direction);
        pos = (pos + direction * dist) % 100;
        if (pos < 0) pos += 100;
    }

    console.log(countZero);
}

main();