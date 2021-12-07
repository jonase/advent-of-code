const std = @import("std");
const input = @embedFile("day07.txt");

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    const allocator = &gpa.allocator;

    const nums: []usize = blk: {
        var list = std.ArrayList(usize).init(allocator);
        var iter = std.mem.split(u8, input, ",");
        while (iter.next()) |num_str| {
            try list.append(try std.fmt.parseInt(usize, num_str, 10));
        }
        break :blk list.toOwnedSlice();
    };
    defer allocator.free(nums);

    var max = std.mem.max(usize, nums);
    var fuel_requirements_part_1 = try allocator.alloc(usize, max);
    var fuel_requirements_part_2 = try allocator.alloc(usize, max);

    for (fuel_requirements_part_1) |*fuel_requirement, p| {
        var sum: usize = 0;
        for (nums) |n| {
            sum += if (n > p) n - p else p - n;
        }
        fuel_requirement.* = sum;
    }

    for (fuel_requirements_part_2) |*fuel_requirement, p| {
        var sum: usize = 0;
        for (nums) |n| {
            const len = if (n > p) n - p else p - n;
            sum += (len * (len + 1)) / 2;
        }
        fuel_requirement.* = sum;
    }

    std.debug.print("{d}\n", .{std.mem.min(usize, fuel_requirements_part_1)});
    std.debug.print("{d}\n", .{std.mem.min(usize, fuel_requirements_part_2)});
}
