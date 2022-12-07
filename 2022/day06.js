#!/usr/bin/env node

const fs = require("fs");

const input = fs.readFileSync("day06.txt", "utf-8").split("");

function allDistinct(arr) {
  return new Set(arr).size === arr.length;
}

function findMarker(size) {
  let i = size;
  while (true) {
    if (allDistinct(input.slice(i - size, i))) break;
    i++;
  }
  return i;
}

console.log("Part 1:", findMarker(4));
console.log("Part 2:", findMarker(14));
