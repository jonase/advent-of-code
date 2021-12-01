const std = @import("std");
const print = std.debug.print;
const input = @embedFile("day01.txt");

fn increaseCount(measurements: []i32) i32 {
    var increase_count: i32 = 0;
    var previous_measurement = measurements[0];
    for (measurements[1..]) |measurement| {
        if (measurement > previous_measurement) {
            increase_count += 1;
        }
        previous_measurement = measurement;
    }
    return increase_count;
}

pub fn main() !void {
    const line_count = 2000;
    var measurements: [line_count]i32 = undefined;

    var idx: usize = 0;
    var lines = std.mem.split(u8, input, "\n");
    while (lines.next()) |measurement| : (idx += 1) {
        measurements[idx] = try std.fmt.parseInt(i32, measurement, 10);
    }

    idx = 0;
    var sliding_sums: [line_count - 2]i32 = undefined;
    while (idx < line_count - 2) : (idx += 1) {
        sliding_sums[idx] = measurements[idx] + measurements[idx + 1] + measurements[idx + 2];
    }

    print("Part 1: {d}\n", .{increaseCount(measurements[0..])});
    print("Part 2: {d}\n", .{increaseCount(sliding_sums[0..])});
}
