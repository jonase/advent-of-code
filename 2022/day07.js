#!/usr/bin/env node

const fs = require("fs");
const assert = require("assert");

let input = fs.readFileSync("day07.txt", "utf-8");

function mkdir(parent, name) {
  let dir = { name, parent, isDirectory: true, children: [] };
  if (parent != null) {
    parent.children.push(dir);
  }
  return dir;
}

function mkfile(parent, name, size) {
  parent.children.push({ name, parent, size, isDirectory: false });
  return parent;
}

function spaces(n) {
  return Array(2 * n + 1).join(" ");
}

function render(file, level) {
  if (file.isDirectory) {
    console.log(`${spaces(level)} - ${file.name} (dir)`);
    file.children.forEach((element) => render(element, level + 1));
  } else {
    console.log(`${spaces(level)} - ${file.name} (file, size=${file.size})`);
  }
}

function parseNext(input, parent) {
  assert(input[0] === "$");
  const cmdString = input.substring(2, input.indexOf("\n"));

  if (cmdString.startsWith("cd")) {
    const nextInput = input.substring(input.indexOf("\n") + 1);

    let [_, dir] = cmdString.split(" ");
    if (dir == "..") {
      return [parent.parent, nextInput];
    } else {
      return [mkdir(parent, dir), nextInput];
    }
  } else if (cmdString.startsWith("ls")) {
    const rest = input.substring(input.indexOf("\n") + 1);
    const startOfNextCommand = rest.indexOf("$");
    const nextInput =
      startOfNextCommand < 0 ? null : rest.substring(startOfNextCommand);

    let lsOutput =
      startOfNextCommand < 0 ? rest : rest.substring(0, rest.indexOf("$") - 1);

    for (const line of lsOutput.split("\n")) {
      if (!line.startsWith("dir")) {
        const [size, name] = line.split(" ");
        mkfile(parent, name, parseInt(size, 10));
      }
    }

    return [parent, nextInput];
  } else assert(false);
}

let cwd = null;

while (input != null) {
  let [dir, nextInput] = parseNext(input, cwd);
  input = nextInput;
  cwd = dir;
}

while (true) {
  if (cwd.parent != null) {
    cwd = cwd.parent;
  } else break;
}

function size(dir) {
  assert(dir.isDirectory);
  let totalSize = 0;
  for (let file of dir.children) {
    if (file.isDirectory) {
      totalSize += size(file);
    } else {
      totalSize += file.size;
    }
  }
  return totalSize;
}

function directorySizes(dir) {
  assert(dir.isDirectory);

  let dirs = [{ name: dir.name, size: size(dir) }];

  for (const file of dir.children) {
    if (file.isDirectory) {
      dirs = [...dirs, ...directorySizes(file)];
    }
  }

  return dirs;
}

const dirs = directorySizes(cwd);

let sum = 0;
for (const dir of dirs) {
  if (dir.size < 100000) {
    sum += dir.size;
  }
}
console.log("Part 1:", sum);

const usedSpace = size(cwd);
const unusedSpace = 70000000 - usedSpace;
const spaceToFree = 30000000 - unusedSpace;

let sizeOfDirToFree = usedSpace;
for (const dir of dirs) {
  if (dir.size > spaceToFree && dir.size < sizeOfDirToFree) {
    sizeOfDirToFree = dir.size;
  }
}

console.log("Part 2:", sizeOfDirToFree);
