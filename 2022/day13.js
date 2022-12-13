#!/usr/bin/env node

const fs = require("fs");

const pairs = fs
  .readFileSync("day13.txt", "utf-8")
  .split("\n\n")
  .map((pair) => {
    let [left, right] = pair.split("\n");
    return [JSON.parse(left), JSON.parse(right)];
  });

function compare(left, right) {
  if (typeof left === "number") {
    if (typeof right === "number") {
      if (left < right) return -1;
      else if (left > right) return 1;
      else return 0;
    } else {
      return compare([left], right);
    }
  } else {
    if (typeof right === "number") {
      return compare(left, [right]);
    } else {
      if (left.length === 0) {
        if (right.length === 0) return 0;
        else return -1;
      } else if (right.length === 0) {
        return 1;
      } else {
        const next = compare(left[0], right[0]);
        if (next === 0) return compare(left.slice(1), right.slice(1));
        else return next;
      }
    }
  }
}

function part1() {
  let indices = [];
  let index = 1;
  for (const [left, right] of pairs) {
    if (compare(left, right) === -1) {
      indices.push(index);
    }
    index++;
  }

  function sum(arr) {
    let s = 0;
    for (const x of arr) s += x;
    return s;
  }

  console.log("Part 1:", sum(indices));
}

function part2() {
  let arr = [[[2]], [[6]]];

  for (const [left, right] of pairs) {
    arr.push(left);
    arr.push(right);
  }

  arr.sort(compare);

  let decoderKey1 = 0;
  let decoderKey2 = 0;
  let index = 1;

  for (const val of arr) {
    const s = JSON.stringify(val);
    if (s === "[[2]]") decoderKey1 = index;
    if (s === "[[6]]") decoderKey2 = index;
    index++;
  }

  console.log("Part 2:", decoderKey1 * decoderKey2);
}

part1();
part2();
