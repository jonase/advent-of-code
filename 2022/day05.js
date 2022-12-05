#!/usr/bin/env node

const fs = require("fs");

const input = fs.readFileSync("day05.txt", "utf-8");

function transpose(m) {
  return m[0].map((_, i) => m.map((row) => row[i]));
}

function parseStacks(stacks) {
  stacks.slice(1)[0].length;

  const rows = stacks.reverse().slice(1);

  const cols = transpose(rows.map((s) => s.split("")))
    .filter((col) => /[A-Z]/.test(col[0]))
    .map((col) => col.join("").replaceAll(" ", "").split(""));

  return cols;
}

function parseMoves(moves) {
  return moves.map((move) => {
    let [_, count, from, to] = move.match(/move (\d+) from (\d+) to (\d+)/);
    return [parseInt(count, 10), parseInt(from, 10) - 1, parseInt(to, 10) - 1];
  });
}

function parse(input) {
  let [stacks, moves] = input.split("\n\n").map((s) => s.split("\n"));
  return [parseStacks(stacks), parseMoves(moves)];
}

function topString(stacks) {
  return stacks.map((stack) => stack[stack.length - 1]).join("");
}

function part1() {
  function moveItem(stacks, from, to) {
    const x = stacks[from].pop();
    stacks[to].push(x);
  }

  let [stacks, moves] = parse(input);

  for (const move of moves) {
    let [count, from, to] = move;

    while (count > 0) {
      moveItem(stacks, from, to);
      count--;
    }
  }

  console.log("Part 1:", topString(stacks));
}

function part2() {
  function moveItems(stacks, count, from, to) {
    const top = stacks[from].splice(stacks[from].length - count);
    stacks[to] = stacks[to].concat(top);
  }

  let [stacks, moves] = parse(input);

  for (const move of moves) {
    let [count, from, to] = move;
    moveItems(stacks, count, from, to);
  }

  console.log("Part 2:", topString(stacks));
}

part1();
part2();
