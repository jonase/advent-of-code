#!/usr/bin/env node

const fs = require("fs");

const input = fs
  .readFileSync("day15.txt", "utf-8")
  .split("\n")
  .map((line) => {
    const [_, sx, sy, bx, by] = line.match(
      /Sensor at x=(-?\d+), y=(-?\d+): closest beacon is at x=(-?\d+), y=(-?\d+)/
    );
    return { sensor: [+sx, +sy], beacon: [+bx, +by] };
  });

function dist([x1, y1], [x2, y2]) {
  return Math.abs(x1 - x2) + Math.abs(y1 - y2);
}

function insertRange(covered, [a, b]) {
  if (b - a < 0) return covered;
  if (covered.length === 0) {
    return [[a, b]];
  }
  const [[x, y], ...rest] = covered;

  if (b + 1 < x) {
    return [[a, b], [x, y], ...rest];
  }

  if (a - 1 > y) {
    return [[x, y], ...insertRange(rest, [a, b])];
  }

  return insertRange(rest, [Math.min(a, x), Math.max(b, y)]);
}

function invertRanges(ranges, [a, b]) {
  let invertedRanges = [];
  for (const range of ranges) {
    invertedRanges = insertRange(invertedRanges, [a, range[0] - 1]);
    a = range[1] + 1;
  }
  return insertRange(invertedRanges, [a, b]);
}

function yRange({ sensor, beacon }, y) {
  const d = dist(sensor, beacon);
  const [sx, sy] = sensor;
  const a = d - Math.abs(sy - y);
  return [sx - a, sx + a];
}

function xRange({ sensor, beacon }, x) {
  const d = dist(sensor, beacon);
  const [sx, sy] = sensor;
  const a = d - Math.abs(sx - x);
  return [sy - a, sy + a];
}

function coveredInRow(y) {
  let covered = [];
  for (const { sensor, beacon } of input) {
    const range = yRange({ sensor, beacon }, y);
    covered = insertRange(covered, range);
  }
  return covered;
}

function coveredInColumn(x) {
  let covered = [];
  for (const { sensor, beacon } of input) {
    const range = xRange({ sensor, beacon }, x);
    covered = insertRange(covered, range);
  }
  return covered;
}

function part1() {
  const y = 2000000;
  const covered = coveredInRow(y);

  let beaconXs = new Set();

  for (const {
    beacon: [bx, by],
  } of input) {
    if (by === y) beaconXs.add(bx);
  }

  let count = 0;
  for (const [lo, hi] of covered) {
    let beaconsInRange = 0;

    for (const bx of beaconXs) {
      if (bx >= lo && bx <= hi) beaconsInRange++;
    }

    count += hi - lo + 1 - beaconsInRange;
  }

  console.log("Part 1:", count);
}

part1();

function part2() {
  for (let y = 0; y <= 4000000; y++) {
    let covered = coveredInRow(y);
    const gaps = invertRanges(covered, [0, 4000000]);
    for (const gap of gaps) {
      for (const x of gap) {
        let coveredCol = coveredInColumn(x);
        let colGaps = invertRanges(coveredCol, [0, 4000000]);
        if (colGaps.length !== 0) {
          console.log("Part 2:", x * 4000000 + colGaps[0][1]);
          return;
        }
      }
    }
  }
}

part2();
