const std = @import("std");
const print = std.debug.print;
const input = @embedFile("day02.txt");

const Op = enum { forward, up, down };

const Command = struct {
    op: Op,
    val: u32,
};

fn parseCommand(command_str: []const u8) !Command {
    var command_iter = std.mem.split(u8, command_str, " ");
    const op_str = command_iter.next().?;
    const op = if (std.mem.eql(u8, op_str, "forward"))
        Op.forward
    else if (std.mem.eql(u8, op_str, "down"))
        Op.down
    else if (std.mem.eql(u8, op_str, "up"))
        Op.up
    else
        unreachable;

    return Command{
        .op = op,
        .val = try std.fmt.parseUnsigned(u32, command_iter.next().?, 10),
    };
}

pub fn main() !void {
    {
        var depth: u32 = 0;
        var forward: u32 = 0;

        var lines = std.mem.split(u8, input, "\n");
        while (lines.next()) |command_str| {
            const command = try parseCommand(command_str);

            switch (command.op) {
                .forward => forward += command.val,
                .down => depth += command.val,
                .up => depth -= command.val,
            }
        }
        print("Part 1: {d}\n", .{depth * forward});
    }

    {
        var aim: u32 = 0;
        var depth: u32 = 0;
        var forward: u32 = 0;

        var lines = std.mem.split(u8, input, "\n");
        while (lines.next()) |command_str| {
            const command = try parseCommand(command_str);

            switch (command.op) {
                .forward => {
                    forward += command.val;
                    depth += aim * command.val;
                },
                .down => aim += command.val,
                .up => aim -= command.val,
            }
        }
        print("Part 2: {d}\n", .{depth * forward});
    }
}
