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
  let commonItems = [];

  for (const rugsack of rugsacks) {
    const compartmentA = new Set(rugsack.slice(0, rugsack.length / 2));
    const compartmentB = new Set(rugsack.slice(rugsack.length / 2));

    commonItems.push(intersection(compartmentA, compartmentB));
  }

  let sum = 0;
  for (const items of commonItems) {
    for (const item of items) {
      sum += priority(item);
    }
  }

  console.log("Part 1:", sum);
}

function part2() {
  const n = rugsacks.length;
  const items = [];
  for (let i = 0; i < n / 3; i++) {
    const rugsack1 = new Set(rugsacks[i * 3 + 0]);
    const rugsack2 = new Set(rugsacks[i * 3 + 1]);
    const rugsack3 = new Set(rugsacks[i * 3 + 2]);

    let common = intersection(intersection(rugsack1, rugsack2), rugsack3);
    items.push([...common][0]);
  }

  let sum = 0;
  for (const item of items) {
    sum += priority(item);
  }

  console.log("Part 2:", sum);
}

part1();
part2();
