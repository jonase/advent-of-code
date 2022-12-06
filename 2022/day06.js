#!/usr/bin/env node

const fs = require("fs");

const input = fs.readFileSync("day06.txt", "utf-8").split("");

function allDistinct(arr) {
  return new Set(arr).size === arr.length;
}

let i = 4;
while (true) {
  if (allDistinct(input.slice(i - 4, i))) break;
  i++;
}

console.log("Part 1:", i);

i = 14;
while (true) {
  if (allDistinct(input.slice(i - 14, i))) break;
  i++;
}

console.log("Part 2:", i);
