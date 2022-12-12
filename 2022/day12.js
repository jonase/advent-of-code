#!/usr/bin/env node

const fs = require("fs");

let start = [0, 0];
let end = [0, 0];
const heightMap = fs
  .readFileSync("day12.txt", "utf-8")
  .split("\n")
  .map((row, r) => {
    return row.split("").map((char, c) => {
      if (char === "S") start = [r, c];
      if (char === "E") {
        end = [r, c];
        char = "{";
      }
      return { char, visited: false, pos: [r, c] };
    });
  });

const rowCount = heightMap.length;
const colCount = heightMap[0].length;

function eql([a, b], [c, d]) {
  return a === c && b === d;
}

function add([r, c], [dr, dc]) {
  return [r + dr, c + dc];
}

function outOfBounds([r, c]) {
  return r < 0 || r >= rowCount || c < 0 || r >= colCount;
}

function get(heightMap, [r, c]) {
  if (outOfBounds([r, c])) return null;
  return heightMap[r][c];
}

function neighbors(heightMap, pos) {
  const curr = get(heightMap, pos);

  const all = [
    get(heightMap, add(pos, [-1, 0])),
    get(heightMap, add(pos, [1, 0])),
    get(heightMap, add(pos, [0, -1])),
    get(heightMap, add(pos, [0, 1])),
  ];
  return all.filter((val) => {
    if (val == null) return false;
    if (val.visited) return false;
    if (curr.char === "S") return true;
    if (val.char.charCodeAt() - curr.char.charCodeAt() > 1) return false;
    return true;
  });
}

function didReachEnd(frontier, end) {
  for (const tile of frontier) {
    if (eql(tile.pos, end)) return true;
  }
  return false;
}

function stepCount(heightMap, startingTile) {
  for (const row of heightMap) {
    for (const tile of row) {
      tile.visited = false;
    }
  }

  let frontier = new Set([startingTile]);
  let steps = 0;

  while (!didReachEnd(frontier, end)) {
    if (frontier.size === 0) return Infinity;
    steps++;
    let newFrontier = new Set();
    for (const tile of frontier) {
      tile.visited = true;
      for (const frontierTile of neighbors(heightMap, tile.pos)) {
        newFrontier.add(frontierTile);
      }
    }
    frontier = newFrontier;
  }

  return steps;
}

function part1(heightMap) {
  console.log("Part 1:", stepCount(heightMap, get(heightMap, start)));
}

function part2(heightMap) {
  const startingTiles = [];
  for (const row of heightMap) {
    for (const tile of row) {
      if (tile.char === "a") {
        startingTiles.push(tile);
      }
    }
  }

  let minSteps = Infinity;

  for (const startingTile of startingTiles) {
    const steps = stepCount(heightMap, startingTile);
    if (steps < minSteps) minSteps = steps;
  }

  console.log("Part 2:", minSteps);
}

part1(heightMap);
part2(heightMap);
