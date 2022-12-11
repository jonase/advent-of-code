#!/usr/bin/env node

const fs = require("fs");

const input = fs.readFileSync("day11.txt", "utf-8");

function parse(input, part) {
  let prod = 1;

  return input.split("\n\n").map((data) => {
    const [_, itemsStr, operationStr, testStr, ifTrueStr, ifFalseStr] =
      data.split("\n");

    const items = itemsStr
      .substring(18)
      .split(", ")
      .map((n) => parseInt(n, 10));

    const [lhsStr, op, rhsStr] = operationStr.substring(19).split(" ");

    const operation = (old) => {
      const lhs = lhsStr === "old" ? old : parseInt(lhsStr, 10);
      const rhs = rhsStr === "old" ? old : parseInt(rhsStr, 10);
      const res = op === "+" ? lhs + rhs : lhs * rhs;
      return part === 1 ? Math.floor(res / 3) : res % prod;
    };

    const test = parseInt(testStr.substring(21), 10);
    prod *= test;

    const trueTarget = parseInt(ifTrueStr.substring(28), 10);

    const falseTarget = parseInt(ifFalseStr.substring(29), 10);

    return {
      items,
      operation,
      test: (n) => n % test === 0,
      target: (b) => (b ? trueTarget : falseTarget),
      inspectCount: 0,
    };
  });
}

function performRound(monkeys) {
  for (const monkey of monkeys) {
    for (const item of monkey.items) {
      monkey.inspectCount++;
      const worryLevel = monkey.operation(item);
      monkeys[monkey.target(monkey.test(worryLevel))].items.push(worryLevel);
      monkey.items = monkey.items.slice(1);
    }
  }
}

function inspectProd(monkeys) {
  const [x, y] = monkeys.map((m) => m.inspectCount).sort((a, b) => b - a);
  return x * y;
}

function part1() {
  const monkeys = parse(input, 1);

  for (let round = 0; round < 20; round++) {
    performRound(monkeys);
  }

  console.log("Part 1:", inspectProd(monkeys));
}

function part2() {
  const monkeys = parse(input, 2);

  for (let round = 0; round < 10000; round++) {
    performRound(monkeys);
  }

  console.log("Part 2:", inspectProd(monkeys));
}

part1();
part2();
