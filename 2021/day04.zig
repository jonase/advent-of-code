const std = @import("std");
const input = @embedFile("day04.txt");

const Board = struct {
    grid: [5][5]u8,
    marked: u25,
};

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();

    const allocator = gpa.allocator();

    var iter = std.mem.split(u8, input, "\n\n");

    const nums: []u8 = result: {
        var list = std.ArrayList(u8).init(allocator);
        var nums_iter = std.mem.split(u8, iter.next().?, ",");
        while (nums_iter.next()) |num_str| {
            try list.append(try std.fmt.parseInt(u8, num_str, 10));
        }
        break :result list.toOwnedSlice();
    };
    defer allocator.free(nums);

    const boards: []Board = result: {
        var list = std.ArrayList(Board).init(allocator);
        while (iter.next()) |board_str| {
            var board: Board = undefined;
            try parseBoard(board_str, &board);
            try list.append(board);
        }
        break :result list.toOwnedSlice();
    };
    defer allocator.free(boards);

    var bingo_count: u32 = 0;
    var first_found_board: Board = undefined;
    var first_found_num: u8 = undefined;
    var last_found_board: Board = undefined;
    var last_found_num: u8 = undefined;

    for (nums) |num| {
        for (boards) |*board| {
            if (isBingo(board.marked)) continue;
            markNum(board, num);
            if (isBingo(board.marked)) {
                bingo_count += 1;
                if (bingo_count == 1) {
                    first_found_board = board.*;
                    first_found_num = num;
                }
                if (bingo_count == boards.len) {
                    last_found_board = board.*;
                    last_found_num = num;
                }
            }
        }
    }

    std.debug.print("Part 1: {d}\n", .{countUnmarked(first_found_board) * first_found_num});
    std.debug.print("Part 2: {d}\n", .{countUnmarked(last_found_board) * last_found_num});
}

fn parseBoard(board_str: []const u8, board: *Board) !void {
    var rows = std.mem.split(u8, board_str, "\n");
    var row_idx: usize = 0;
    while (rows.next()) |row_str| : (row_idx += 1) {
        var cols = std.mem.tokenize(u8, row_str, " ");
        var col_idx: usize = 0;
        while (cols.next()) |col_str| : (col_idx += 1) {
            const n = try std.fmt.parseInt(u8, col_str, 10);
            board.grid[row_idx][col_idx] = n;
        }
    }
    board.marked = 0;
}

fn printBoard(board: Board) void {
    for (board.grid) |row, row_idx| {
        for (row) |n, col_idx| {
            if (isMarked(board.marked, row_idx, col_idx)) {
                std.debug.print("\x1b[41;1m{d:2}\x1b[0m ", .{n});
            } else {
                std.debug.print("{d:2} ", .{n});
            }
        }
        std.debug.print("\n", .{});
    }
}

fn isMarked(marked: u25, row_idx: usize, col_idx: usize) bool {
    var nbit = row_idx * 5 + col_idx;
    return marked & @as(u25, 1) << @intCast(u5, nbit) != 0;
}

fn markNum(board: *Board, num: u8) void {
    for (board.grid) |row, row_idx| {
        for (row) |n, col_idx| {
            if (n == num) {
                mark(&board.marked, row_idx, col_idx);
            }
        }
    }
}

fn mark(marked: *u25, row_idx: usize, col_idx: usize) void {
    var nbit = row_idx * 5 + col_idx;
    marked.* |= @as(u25, 1) << @intCast(u5, nbit);
}

const bingo_patterns = [_]u25{
    0b11111_00000_00000_00000_00000,
    0b00000_11111_00000_00000_00000,
    0b00000_00000_11111_00000_00000,
    0b00000_00000_00000_11111_00000,
    0b00000_00000_00000_00000_11111,
    0b10000_10000_10000_10000_10000,
    0b01000_01000_01000_01000_01000,
    0b00100_00100_00100_00100_00100,
    0b00010_00010_00010_00010_00010,
    0b00001_00001_00001_00001_00001,
};

fn isBingo(marked: u25) bool {
    for (bingo_patterns) |bingo_pattern| {
        if (marked & bingo_pattern == bingo_pattern) return true;
    }
    return false;
}

fn countUnmarked(board: Board) u32 {
    var unmarked_sum: u32 = 0;
    for (board.grid) |row, row_idx| {
        for (row) |n, col_idx| {
            if (!isMarked(board.marked, row_idx, col_idx)) {
                unmarked_sum += n;
            }
        }
    }
    return unmarked_sum;
}
