#!/usr/bin/env node

const fs = require("fs");

const paths = fs
  .readFileSync("day14.txt", "utf-8")
  .split("\n")
  .map((line) => {
    return line.split(" -> ").map((pair) => {
      const [x, y] = pair.split(",");
      return [parseInt(x, 10), parseInt(y, 10)];
    });
  });

let [minX, maxX] = [Infinity, 0];
let [minY, maxY] = [0, 0];

for (const path of paths) {
  for (const [x, y] of path) {
    if (x < minX) minX = x;
    if (x > maxX) maxX = x;
    if (y > maxY) maxY = y;
  }
}

function add([x, y], [dx, dy]) {
  return [x + dx, y + dy];
}

function eql([x1, y1], [x2, y2]) {
  return x1 === x2 && y1 === y2;
}

function outOfBounds(grid, [x, y]) {
  return y < 0 || y >= grid.length || x < 0 || x >= grid[0].length;
}

function get(grid, [x, y]) {
  return grid[y][x];
}

function drop(grid, sandGrain) {
  let down = add(sandGrain, [0, 1]);
  let left = add(sandGrain, [-1, 1]);
  let right = add(sandGrain, [1, 1]);

  if (outOfBounds(grid, down)) return false;
  if (get(grid, down) === 0) return drop(grid, down);
  if (outOfBounds(grid, left)) return false;
  if (get(grid, left) === 0) return drop(grid, left);
  if (outOfBounds(grid, right)) return false;
  if (get(grid, right) === 0) return drop(grid, right);

  grid[sandGrain[1]][sandGrain[0]] = 2;
  return true;
}

function grainStep(start, end) {
  return [Math.sign(end[0] - start[0]), Math.sign(end[1] - start[1])];
}

function grainCount(grid) {
  let sum = 0;
  for (const row of grid) {
    for (const n of row) {
      if (n === 2) sum += 1;
    }
  }
  return sum;
}

function fill(grid, start, end) {
  const step = grainStep(start, end);

  let cursor = start;
  while (!eql(cursor, end)) {
    grid[cursor[1]][cursor[0]] = 1;
    cursor = add(cursor, step);
  }
  grid[end[1]][end[0]] = 1;
}

function part1() {
  let grid = Array(maxY - minY + 1).fill([]);

  for (let i in grid) {
    grid[i] = Array(maxX - minX + 1).fill(0);
  }

  for (const path of paths) {
    for (let i = 0; i < path.length - 1; i++) {
      const start = add(path[i], [-minX, 0]);
      const end = add(path[i + 1], [-minX, 0]);
      fill(grid, start, end);
    }
  }

  const sandStart = [500 - minX, 0];

  while (true) {
    let sandGrain = sandStart;
    if (!drop(grid, sandGrain)) break;
  }

  console.log("Part 1:", grainCount(grid));
}

part1();

function part2() {
  let grid = Array(maxY + 3).fill([]);

  for (let i in grid) {
    grid[i] = Array(maxX * 2 + 3).fill(0);
  }

  grid[grid.length - 1] = Array(maxX * 2 + 3).fill(1);

  for (const path of paths) {
    for (let i = 0; i < path.length - 1; i++) {
      const start = path[i];
      const end = path[i + 1];
      fill(grid, start, end);
    }
  }

  const sandStart = [500, 0];

  while (true) {
    let sandGrain = sandStart;
    if (!drop(grid, sandGrain)) break;
    if (grid[0][500] === 2) break;
  }

  console.log("Part 2:", grainCount(grid));
}

part2();
