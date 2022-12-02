#!/usr/bin/env node

const fs = require("fs");

const input = fs.readFileSync("day02.txt", "utf-8").split("\n");

function part1() {
  const scores = {
    "A X": 1 + 3,
    "A Y": 2 + 6,
    "A Z": 3 + 0,
    "B X": 1 + 0,
    "B Y": 2 + 3,
    "B Z": 3 + 6,
    "C X": 1 + 6,
    "C Y": 2 + 0,
    "C Z": 3 + 3,
  };

  let sum = 0;

  for (const s of input) {
    sum += scores[s];
  }

  console.log("Part 1", sum);
}

function part2() {
  const win_scores = { A: 2, B: 3, C: 1 };
  const loss_scores = { A: 3, B: 1, C: 2 };
  const draw_scores = { A: 1, B: 2, C: 3 };

  let sum = 0;
  for (const s of input) {
    switch (s[2]) {
      case "X":
        sum += 0 + loss_scores[s[0]];
        break;
      case "Y":
        sum += 3 + draw_scores[s[0]];
        break;
      case "Z":
        sum += 6 + win_scores[s[0]];
        break;
    }
  }
  console.log("Part 2", sum);
}

part1();
part2();
