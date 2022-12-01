const std = @import("std");

const input = @embedFile("day09.txt");

const column_count = std.mem.indexOf(u8, input, "\n").?;
const row_count = blk: {
    @setEvalBranchQuota(50000);
    break :blk std.mem.count(u8, input, "\n") + 1;
};

var heat_map: [row_count][column_count]u8 = undefined;

const HeatMapIndex = struct {
    row: isize,
    col: isize,
};

fn getHeight(index: HeatMapIndex) ?u8 {
    if (index.row < 0 or index.row >= row_count or index.col < 0 or index.col >= column_count) {
        return null;
    }
    return heat_map[@intCast(usize, index.row)][@intCast(usize, index.col)];
}

pub fn main() !void {
    { // Populate the heat map
        var rows = std.mem.split(u8, input, "\n");
        var row_idx: usize = 0;
        while (rows.next()) |row| : (row_idx += 1) {
            for (row) |ch, col_idx| {
                heat_map[row_idx][col_idx] = ch - '0';
            }
        }
    }

    { // Part 1
        var adjacent_indices: [4]HeatMapIndex = undefined;

        var sum: usize = 0;

        for (heat_map) |row, row_idx| {
            const i_row_idx = @intCast(isize, row_idx);
            for (row) |height, col_idx| {
                const i_col_idx = @intCast(isize, col_idx);
                adjacent_indices[0] = .{ .row = i_row_idx - 1, .col = i_col_idx };
                adjacent_indices[1] = .{ .row = i_row_idx + 1, .col = i_col_idx };
                adjacent_indices[2] = .{ .row = i_row_idx, .col = i_col_idx - 1 };
                adjacent_indices[3] = .{ .row = i_row_idx, .col = i_col_idx + 1 };
                var is_low_point = true;
                for (adjacent_indices) |adjacent_index| {
                    if (getHeight(adjacent_index)) |adjacent_height| {
                        if (height >= adjacent_height) {
                            is_low_point = false;
                            break;
                        }
                    }
                }
                if (is_low_point) {
                    sum += height + 1;
                }
            }
        }

        std.debug.print("Part 1: {d}\n", .{sum});
    }

    { // Part 2
        var gpa = std.heap.GeneralPurposeAllocator(.{}){};
        defer _ = gpa.deinit();
        const allocator = gpa.allocator();

        var basin_sizes = std.ArrayList(usize).init(allocator);
        defer basin_sizes.deinit();

        for (heat_map) |row, row_idx| {
            for (row) |_, col_idx| {
                if (heat_map[row_idx][col_idx] == 9) {
                    continue;
                } else {
                    const size = basinSize(row_idx, col_idx);
                    try basin_sizes.append(size);
                }
            }
        }

        std.sort.sort(usize, basin_sizes.items, {}, std.sort.desc(usize));

        std.debug.print("Part 2: {d}\n", .{basin_sizes.items[0] * basin_sizes.items[1] * basin_sizes.items[2]});
    }
}

fn basinSize(row_idx: usize, col_idx: usize) usize {
    var size: usize = 1;
    heat_map[row_idx][col_idx] = 9;

    // left
    if (row_idx > 0 and heat_map[row_idx - 1][col_idx] != 9) {
        size += basinSize(row_idx - 1, col_idx);
    }

    // right
    if (row_idx < row_count - 1 and heat_map[row_idx + 1][col_idx] != 9) {
        size += basinSize(row_idx + 1, col_idx);
    }

    // up
    if (col_idx > 0 and heat_map[row_idx][col_idx - 1] != 9) {
        size += basinSize(row_idx, col_idx - 1);
    }

    // down
    if (col_idx < column_count - 1 and heat_map[row_idx][col_idx + 1] != 9) {
        size += basinSize(row_idx, col_idx + 1);
    }

    return size;
}
