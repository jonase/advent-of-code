#!/usr/bin/env node

const fs = require("fs");

function transpose(m) {
  return m[0].map((_, i) => m.map((row) => row[i]));
}

const grid = fs
  .readFileSync("day08.txt", "utf-8")
  .split("\n")
  .map((r) => r.split("").map((n) => parseInt(n, 10)));

const colGrid = transpose(grid);

const size = grid.length;

function max(arr) {
  let max = arr[0];
  for (const n of arr.slice(1)) {
    if (n > max) max = n;
  }
  return max;
}

function isVisible(r, c) {
  const n = grid[r][c];
  let row = grid[r];
  let col = colGrid[c];

  return (
    max(row.slice(0, c)) < n ||
    max(row.slice(c + 1)) < n ||
    max(col.slice(0, r)) < n ||
    max(col.slice(r + 1)) < n
  );
}

let count = 0;
for (let i = 1; i < size - 1; i++) {
  for (let j = 1; j < size - 1; j++) {
    if (isVisible(i, j)) count++;
  }
}

console.log("Part 1:", count + 4 * size - 4);

function viewingDistance(n, arr) {
  let d = 0;
  for (const x of arr) {
    d++;
    if (x >= n) break;
  }
  return d;
}

function scenicScore(r, c) {
  const n = grid[r][c];
  let row = grid[r];
  let col = colGrid[c];

  return (
    viewingDistance(n, row.slice(0, c).reverse()) *
    viewingDistance(n, row.slice(c + 1)) *
    viewingDistance(n, col.slice(0, r).reverse()) *
    viewingDistance(n, col.slice(r + 1))
  );
}

let maxScore = 0;

for (let i = 1; i < size - 1; i++) {
  for (let j = 1; j < size - 1; j++) {
    const score = scenicScore(i, j);
    if (score > maxScore) maxScore = score;
  }
}

console.log("Part 2:", maxScore);
