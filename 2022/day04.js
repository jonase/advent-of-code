#!/usr/bin/env node

const fs = require("fs");

function parse(s) {
  const pairs = s.split(",");
  return [
    pairs[0].split("-").map((x) => parseInt(x, 10)),
    pairs[1].split("-").map((x) => parseInt(x, 10)),
  ];
}

const input = fs.readFileSync("day04.txt", "utf-8").split("\n").map(parse);

function isOverlapping([[lo1, hi1], [lo2, hi2]]) {
  return (lo1 <= lo2 && hi1 >= hi2) || (lo2 <= lo1 && hi2 >= hi1);
}

console.log("Part 1:", input.filter(isOverlapping).length);

function isDistinct([[lo1, hi1], [lo2, hi2]]) {
  return hi1 < lo2 || hi2 < lo1;
}

console.log("Part 2:", input.length - input.filter(isDistinct).length);
