const std = @import("std");
const input = @embedFile("day03.txt");

pub fn main() !void {
    {
        // Part 1
        var lines = std.mem.split(u8, input, "\n");
        var bit_counts = [_]u32{0} ** 12;
        var line_count: usize = 0;
        while (lines.next()) |bit_pattern| : (line_count += 1) {
            for (bit_pattern) |bit, idx| {
                if (bit == '1') bit_counts[idx] += 1;
            }
        }

        var gamma_rate: u12 = 0;
        for (bit_counts) |bit_count, idx| {
            if (bit_count > line_count / 2) {
                gamma_rate |= @as(u12, 1) << (11 - @intCast(u4, idx));
            }
        }

        const epsilon_rate: u12 = ~gamma_rate;
        std.debug.print("Part 1: {d}\n", .{@as(u32, epsilon_rate) * gamma_rate});
    }

    {
        // Part 2
        var nums: [1000]u12 = undefined;
        {
            var lines = std.mem.split(u8, input, "\n");
            var idx: usize = 0;
            while (lines.next()) |bit_pattern| : (idx += 1) {
                nums[idx] = try std.fmt.parseUnsigned(u12, bit_pattern, 2);
            }
        }

        var oxygen_generator_rating: u12 = result: {
            var nums_copy: [1000]u12 = undefined;
            std.mem.copy(u12, nums_copy[0..], nums[0..]);
            break :result findOxygenGeneratorRating(11, nums_copy[0..])[0];
        };

        var co2_scrubber_rating: u12 = findC02ScrubberRating(11, nums[0..])[0];

        std.debug.print("Part 2: {d}\n", .{@as(u32, oxygen_generator_rating) * co2_scrubber_rating});
    }
}

fn findOxygenGeneratorRating(bit_idx: usize, nums: []u12) []u12 {
    const most_common_bit = mostCommonBit(bit_idx, nums);
    var filtered_nums = filterByBitPrefix(bit_idx, most_common_bit, nums);
    if (filtered_nums.len == 1) return filtered_nums;
    return findOxygenGeneratorRating(bit_idx - 1, filtered_nums);
}

fn findC02ScrubberRating(bit_idx: usize, nums: []u12) []u12 {
    const least_common_bit = leastCommonBit(bit_idx, nums);
    var filtered_nums = filterByBitPrefix(bit_idx, least_common_bit, nums);
    if (filtered_nums.len == 1) return filtered_nums;
    return findC02ScrubberRating(bit_idx - 1, filtered_nums);
}

fn mostCommonBit(bit_idx: usize, nums: []u12) u1 {
    const ones = countOnes(bit_idx, nums);
    const zeros = nums.len - ones;
    return if (ones >= zeros) 1 else 0;
}

fn leastCommonBit(bit_idx: usize, nums: []u12) u1 {
    const ones = countOnes(bit_idx, nums);
    const zeros = nums.len - ones;
    return if (zeros > ones) 1 else 0;
}

fn countOnes(bit_idx: usize, nums: []u12) usize {
    var ones: usize = 0;
    for (nums) |num| {
        if (num & (@as(u12, 1) << @intCast(u4, bit_idx)) != 0) {
            ones += 1;
        }
    }
    return ones;
}

fn filterByBitPrefix(bit_idx: usize, bit: u1, nums: []u12) []u12 {
    var idx: usize = 0;
    for (nums) |num| {
        if (isBitSet(num, bit_idx, bit)) {
            nums[idx] = num;
            idx += 1;
        }
    }
    return nums[0..idx];
}

fn isBitSet(num: u12, bit_idx: usize, bit: u1) bool {
    const bit_result: u1 = if (num & @as(u12, 1) << @intCast(u4, bit_idx) == 0) 0 else 1;
    return bit_result == bit;
}
