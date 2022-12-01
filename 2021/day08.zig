const std = @import("std");

const input = @embedFile("day08.txt");

const Display = struct {
    signals: [10][]const u8,
    outputs: [4][]const u8,
};

fn parseLine(line: []const u8, display: *Display) void {
    var parts = std.mem.split(u8, line, " | ");

    {
        var lhs = parts.next().?;
        var lhs_parts = std.mem.split(u8, lhs, " ");
        var idx: usize = 0;
        while (lhs_parts.next()) |segment| : (idx += 1) {
            //std.sort.sort(u8, segment, {}, std.sort.asc(u8));
            display.signals[idx] = segment;
        }
    }

    {
        var rhs = parts.next().?;
        var rhs_parts = std.mem.split(u8, rhs, " ");
        var idx: usize = 0;
        while (rhs_parts.next()) |segment| : (idx += 1) {
            //std.sort.sort(u8, segment, {}, std.sort.asc(u8));
            display.outputs[idx] = segment;
        }
    }
}

fn isSubset(subset: []const u8, superset: []const u8) bool {
    for (subset) |ch| {
        if (std.mem.indexOfScalar(u8, superset, ch) == null) {
            return false;
        }
    }
    return true;
}

fn eql(a: []const u8, b: []const u8) bool {
    return a.len == b.len and isSubset(a, b);
}

fn outputValue(display: *Display) usize {
    // Map from digit to index in display.signals
    var digits: [10]usize = undefined;

    for (display.signals) |signal, idx| {
        switch (signal.len) {
            2 => digits[1] = idx,
            3 => digits[7] = idx,
            4 => digits[4] = idx,
            5 => continue,
            6 => continue,
            7 => digits[8] = idx,
            else => continue,
        }
    }

    // Observation: 7 is the only subset of 3 (len 5)
    for (display.signals) |signal, idx| {
        switch (signal.len) {
            5 => {
                if (isSubset(display.signals[digits[7]], signal)) {
                    digits[3] = idx;
                    break;
                }
            },
            else => continue,
        }
    }

    // Observation: 3 is the only subset of 9 (len 6)
    for (display.signals) |signal, idx| {
        switch (signal.len) {
            6 => {
                if (isSubset(display.signals[digits[3]], signal)) {
                    digits[9] = idx;
                    break;
                }
            },
            else => continue,
        }
    }

    // Observation: With 9 resolved, 1 is the only subset of 0 (len 6)
    for (display.signals) |signal, idx| {
        switch (signal.len) {
            6 => {
                if (isSubset(display.signals[digits[1]], signal) and !std.mem.eql(u8, signal, display.signals[digits[9]])) {
                    digits[0] = idx;
                    break;
                }
            },
            else => continue,
        }
    }

    // Observation: With 0 and 9 resolved, 6 is the only one left (len 6)
    for (display.signals) |signal, idx| {
        switch (signal.len) {
            6 => {
                if (std.mem.eql(u8, signal, display.signals[digits[9]]) or std.mem.eql(u8, signal, display.signals[digits[0]])) {
                    continue;
                } else {
                    digits[6] = idx;
                    break;
                }
            },
            else => continue,
        }
    }

    // Observation: With 3 resolved, 5 is a subset of 9 (while 2 is not)
    for (display.signals) |signal, idx| {
        switch (signal.len) {
            5 => {
                if (isSubset(signal, display.signals[digits[9]]) and !std.mem.eql(u8, signal, display.signals[digits[3]])) {
                    digits[5] = idx;
                    break;
                }
            },
            else => continue,
        }
    }

    // Observation: With 3 and 5 resolved, 2 is the only len 5 left
    for (display.signals) |signal, idx| {
        switch (signal.len) {
            5 => {
                if (std.mem.eql(u8, signal, display.signals[digits[3]]) or std.mem.eql(u8, signal, display.signals[digits[5]])) {
                    continue;
                } else {
                    digits[2] = idx;
                    break;
                }
            },
            else => continue,
        }
    }

    var output: usize = 0;

    for (display.outputs) |segment, idx| {
        for (digits) |signal_segment, signal_idx| {
            if (eql(segment, display.signals[signal_segment])) {
                output += std.math.pow(usize, 10, 3 - idx) * signal_idx;
            }
        }
    }

    return output;
}

pub fn main() !void {
    var lines = std.mem.split(u8, input, "\n");

    var count: usize = 0;
    var sum: usize = 0;
    while (lines.next()) |line| {
        var display: Display = undefined;
        parseLine(line, &display);

        sum += outputValue(&display);

        for (display.outputs) |output| {
            const len = output.len;
            if (len == 2 or len == 3 or len == 4 or len == 7) {
                count += 1;
            }
        }
    }

    std.debug.print("Part 1: {d}\n", .{count});
    std.debug.print("Part 2: {d}\n", .{sum});
}
