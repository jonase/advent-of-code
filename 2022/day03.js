#!/usr/bin/env node

const fs = require("fs");

const rugsacks = fs
  .readFileSync("day03.txt", "utf-8")
  .split("\n")
  .map((rs) => rs.split(""));

function intersection(set1, set2) {
  return new Set([...set1].filter((item) => set2.has(item)));
}

function priority(item) {
  if (item === item.toLowerCase()) {
    return item.charCodeAt(0) - 96;
  } else {
    return item.charCodeAt(0) - 38;
  }
}

function part1() {
  let sum = 0;
  for (const rugsack of rugsacks) {
    const compartmentA = new Set(rugsack.slice(0, rugsack.length / 2));
    const compartmentB = new Set(rugsack.slice(rugsack.length / 2));

    const common = intersection(compartmentA, compartmentB);
    sum += priority([...common][0]);
  }

  console.log("Part 1:", sum);
}

function part2() {
  let sum = 0;
  for (let i = 0; i < rugsacks.length / 3; i++) {
    const rugsack1 = new Set(rugsacks[i * 3 + 0]);
    const rugsack2 = new Set(rugsacks[i * 3 + 1]);
    const rugsack3 = new Set(rugsacks[i * 3 + 2]);

    let common = intersection(intersection(rugsack1, rugsack2), rugsack3);
    sum += priority([...common][0]);
  }

  console.log("Part 2:", sum);
}

part1();
part2();
