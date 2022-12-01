const std = @import("std");

const input = @embedFile("day11.txt");

pub fn main() void {
    var energy_levels: [10][10]u8 = undefined;

    {
        var lines = std.mem.split(u8, input, "\n");
        var row_idx: usize = 0;
        while (lines.next()) |row| : (row_idx += 1) {
            for (row) |ch, col_idx| {
                energy_levels[row_idx][col_idx] = ch - '0';
            }
        }
    }

    var step: usize = 0;
    var flash_count: usize = 0;
    while (true) : (step += 1) {
        for (energy_levels) |*row| {
            for (row.*) |*energy_level| {
                energy_level.* += 1;
            }
        }

        var some_flashed = true;
        while (some_flashed) {
            some_flashed = false;
            for (energy_levels) |*row, row_idx| {
                for (row.*) |*energy_level, col_idx| {
                    if (energy_level.* > 9) {
                        some_flashed = true;
                        if (step < 100) {
                            flash_count += 1;
                        }
                        energy_level.* = 0;
                        incNeighbours(&energy_levels, row_idx, col_idx);
                    }
                }
            }
        }

        var all_flashed: bool = true;
        for (energy_levels) |row| {
            for (row) |energy_level| {
                if (energy_level != 0) {
                    all_flashed = false;
                    break;
                }
            }
        }

        if (all_flashed) break;
    }
    std.debug.print("Part 1: {d}\n", .{flash_count});
    std.debug.print("Part 2: {d}\n", .{step + 1});
}

fn incIfNotZero(energy_levels: *[10][10]u8, row_idx: usize, col_idx: usize) void {
    if (energy_levels.*[row_idx][col_idx] != 0) {
        energy_levels.*[row_idx][col_idx] += 1;
    }
}

fn incNeighbours(energy_levels: *[10][10]u8, row_idx: usize, col_idx: usize) void {
    // above
    if (row_idx > 0) {
        if (col_idx > 0) incIfNotZero(energy_levels, row_idx - 1, col_idx - 1);
        incIfNotZero(energy_levels, row_idx - 1, col_idx);
        if (col_idx < 9) incIfNotZero(energy_levels, row_idx - 1, col_idx + 1);
    }

    // below
    if (row_idx < 9) {
        if (col_idx > 0) incIfNotZero(energy_levels, row_idx + 1, col_idx - 1);
        incIfNotZero(energy_levels, row_idx + 1, col_idx);
        if (col_idx < 9) incIfNotZero(energy_levels, row_idx + 1, col_idx + 1);
    }

    // left
    if (col_idx > 0) incIfNotZero(energy_levels, row_idx, col_idx - 1);

    // right
    if (col_idx < 9) incIfNotZero(energy_levels, row_idx, col_idx + 1);
}
