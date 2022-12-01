const std = @import("std");

const input = @embedFile("day05.txt");

const Point = struct {
    x: i32,
    y: i32,

    fn eql(p1: Point, p2: Point) bool {
        return p1.x == p2.x and p1.y == p2.y;
    }

    fn add(p1: Point, p2: Point) Point {
        return Point{
            .x = p1.x + p2.x,
            .y = p1.y + p2.y,
        };
    }
};

const LineSegment = struct {
    start: Point,
    end: Point,
};

fn addOverlapping(overlaps: *std.AutoHashMap(Point, usize), line_segment: LineSegment, inc: Point) !void {
    var p = line_segment.start;
    while (!p.eql(line_segment.end)) : (p = p.add(inc)) {
        var res = try overlaps.getOrPut(p);
        if (res.found_existing) {
            res.value_ptr.* += 1;
        } else {
            res.value_ptr.* = 1;
        }
    }

    var res = try overlaps.getOrPut(line_segment.end);
    if (res.found_existing) {
        res.value_ptr.* += 1;
    } else {
        res.value_ptr.* = 1;
    }
}

fn countOverlaps(overlaps: *std.AutoHashMap(Point, usize)) usize {
    var n: usize = 0;
    var iter = overlaps.valueIterator();
    while (iter.next()) |count| {
        if (count.* > 1) n += 1;
    }
    return n;
}

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const allocator = gpa.allocator();

    var line_segments = std.ArrayList(LineSegment).init(allocator);
    defer line_segments.deinit();

    var lines = std.mem.split(u8, input, "\n");

    while (lines.next()) |line_str| {
        var parts = std.mem.split(u8, line_str, " -> ");

        var lhs = std.mem.split(u8, parts.next().?, ",");
        const p1 = Point{
            .x = try std.fmt.parseInt(i32, lhs.next().?, 10),
            .y = try std.fmt.parseInt(i32, lhs.next().?, 10),
        };

        var rhs = std.mem.split(u8, parts.next().?, ",");
        const p2 = Point{
            .x = try std.fmt.parseInt(i32, rhs.next().?, 10),
            .y = try std.fmt.parseInt(i32, rhs.next().?, 10),
        };

        try line_segments.append(LineSegment{ .start = p1, .end = p2 });
    }

    var overlaps = std.AutoHashMap(Point, usize).init(allocator);
    defer overlaps.deinit();

    {
        for (line_segments.items) |line_segment| {
            const inc = Point{
                .x = std.math.sign(line_segment.end.x - line_segment.start.x),
                .y = std.math.sign(line_segment.end.y - line_segment.start.y),
            };

            if (inc.x != 0 and inc.y != 0) continue;

            try addOverlapping(&overlaps, line_segment, inc);
        }

        std.debug.print("Part 1: {d}\n", .{countOverlaps(&overlaps)});
    }

    {
        for (line_segments.items) |line_segment| {
            const inc = Point{
                .x = std.math.sign(line_segment.end.x - line_segment.start.x),
                .y = std.math.sign(line_segment.end.y - line_segment.start.y),
            };

            if (inc.x == 0 or inc.y == 0) continue;

            try addOverlapping(&overlaps, line_segment, inc);
        }

        std.debug.print("Part 2: {d}\n", .{countOverlaps(&overlaps)});
    }
}
