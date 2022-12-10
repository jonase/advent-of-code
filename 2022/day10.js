#!/usr/bin/env node

const fs = require("fs");

const insts = fs
  .readFileSync("day10.txt", "utf-8")
  .split("\n")
  .map((line) => {
    let [op, arg] = line.split(" ");
    return { op, arg: arg && parseInt(arg, 10) };
  });

function part1() {
  let cycle = 0;
  let X = 1;
  let limit = 20;
  let sum = 0;

  for (const inst of insts) {
    const inc = inst.op === "noop" ? 1 : 2;
    if (cycle < limit && cycle + inc >= limit) {
      sum += limit * X;
      limit += 40;
    }
    if (inst.op === "noop") {
      cycle += 1;
    } else {
      cycle += 2;
      X += inst.arg;
    }
  }

  console.log("Part 1:", sum);
}

part1();

function part2() {
  let instIdx = 0;
  let spritePos = 1;
  let skip = true;
  for (let cycle = 1; cycle <= 240; cycle++) {
    const { op, arg } = insts[instIdx];
    const x = cycle % 40;

    process.stdout.write(x >= spritePos && x < spritePos + 3 ? "#" : " ");
    if (x === 0) process.stdout.write("\n");

    if (op === "noop") {
      instIdx++;
    } else if (skip) {
      skip = false;
    } else {
      instIdx++;
      spritePos += arg;
      skip = true;
    }
  }
}

part2();
