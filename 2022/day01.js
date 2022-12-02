#!/usr/bin/env node

const fs = require("fs");

const inputs = fs.readFileSync("day01.txt", "utf-8").split("\n\n");

let sums = [];

for (const input of inputs) {
  const sum = input
    .split("\n")
    .map((n) => parseInt(n, 10))
    .reduce((aggr, n) => aggr + n, 0);

  sums.push(sum);
}

sums.sort((a, b) => b - a);

console.log("Part 1:", sums[0]);
console.log("Part 2:", sums[0] + sums[1] + sums[2]);
