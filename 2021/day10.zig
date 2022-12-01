const std = @import("std");
const input = @embedFile("day10.txt");

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const allocator = gpa.allocator();

    var stack = std.ArrayList(u8).init(allocator);
    defer stack.deinit();

    var lines = std.mem.split(u8, input, "\n");

    var score: usize = 0;

    var incomplete_scores = std.ArrayList(usize).init(allocator);
    defer incomplete_scores.deinit();

    var is_line_invalid = false;

    while (lines.next()) |chunks| {
        for (chunks) |ch| {
            switch (ch) {
                '(', '[', '{', '<' => try stack.append(ch),
                ')' => if (stack.pop() != '(') {
                    is_line_invalid = true;
                    score += 3;
                    continue;
                },
                ']' => if (stack.pop() != '[') {
                    is_line_invalid = true;
                    score += 57;
                    continue;
                },
                '}' => if (stack.pop() != '{') {
                    is_line_invalid = true;
                    score += 1197;
                    continue;
                },
                '>' => if (stack.pop() != '<') {
                    is_line_invalid = true;
                    score += 25137;
                    continue;
                },
                else => unreachable,
            }
        }

        if (!is_line_invalid and stack.items.len != 0) {
            std.mem.reverse(u8, stack.items);
            var incomplete_score: usize = 0;
            for (stack.items) |ch| {
                const ch_score: u8 = switch (ch) {
                    '(' => 1,
                    '[' => 2,
                    '{' => 3,
                    '<' => 4,
                    else => unreachable,
                };
                incomplete_score = 5 * incomplete_score + ch_score;
            }
            try incomplete_scores.append(incomplete_score);
        }

        is_line_invalid = false;
        try stack.resize(0);
    }

    std.debug.print("Part 1: {d}\n", .{score});
    std.sort.sort(usize, incomplete_scores.items, {}, std.sort.asc(usize));
    std.debug.print("Part 2: {d}\n", .{incomplete_scores.items[@divFloor(incomplete_scores.items.len, 2)]});
}
