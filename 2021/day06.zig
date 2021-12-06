const std = @import("std");
const input = @embedFile("day06.txt");

fn simulate(buckets: *[9]u64, days_total: u64) u64 {
    var day: u64 = 0;
    while (day < days_total) {
        const zeroes = buckets[0];
        for (buckets[1..]) |n, idx| {
            buckets[idx] = n;
        }
        buckets[6] += zeroes;
        buckets[8] = zeroes;
        day += 1;
    }

    var sum: u64 = 0;
    for (buckets) |n| {
        sum += n;
    }
    return sum;
}

pub fn main() !void {
    var buckets = [_]u64{0} ** 9;
    var iter = std.mem.split(u8, input, ",");
    while (iter.next()) |str| {
        const idx = try std.fmt.parseUnsigned(u64, str, 10);
        buckets[idx] += 1;
    }

    const sum_part1 = simulate(&buckets, 80);
    const sum_part2 = simulate(&buckets, 256 - 80);

    std.debug.print("Part 1: {d}\n", .{sum_part1});
    std.debug.print("Part 2: {d}\n", .{sum_part2});
}
