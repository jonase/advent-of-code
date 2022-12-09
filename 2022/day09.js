#!/usr/bin/env node

const fs = require("fs");

const dirMap = { U: [0, -1], D: [0, 1], L: [-1, 0], R: [1, 0] };

const input = fs
  .readFileSync("day09.txt", "utf-8")
  .split("\n")
  .map((line) => {
    let [dir, n] = line.split(" ");
    return [dirMap[dir], parseInt(n, 10)];
  });

function addVisited(visited, [x, y]) {
  visited.add(`${x},${y}`);
}

function add([x, y], [dx, dy]) {
  return [x + dx, y + dy];
}

function tailMove(head, tail) {
  const dx = head[0] - tail[0];
  const dy = head[1] - tail[1];

  if (Math.sqrt(dx * dx + dy * dy) < 2) {
    return [0, 0];
  } else {
    return [Math.sign(dx), Math.sign(dy)];
  }
}

function last(arr) {
  return arr[arr.length - 1];
}

function step(dir, knots, visited) {
  const newKnots = [add(knots[0], dir)];

  for (const knot of knots.slice(1)) {
    newKnots.push(add(knot, tailMove(last(newKnots), knot)));
  }

  addVisited(visited, last(newKnots));
  return newKnots;
}

function move([dir, n], knots, visited) {
  if (n === 0) return knots;
  let newKnots = step(dir, knots, visited);
  return move([dir, n - 1], newKnots, visited);
}

function solve(knots) {
  const visited = new Set();

  for (let m of input) {
    knots = move(m, knots, visited);
  }

  return visited.size;
}

console.log("Part 1:", solve(Array(2).fill([0, 0])));
console.log("Part 2:", solve(Array(10).fill([0, 0])));
