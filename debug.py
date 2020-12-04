import lldb
import os
import re
import subprocess

filename = "out/.day"

def __lldb_init_module(debugger, internal_dict):
    debugger.HandleCommand('command script add -f debug.day day')
    debugger.HandleCommand('command script add -f debug.compile cc')


def day(debugger, command, exe_ctx, result, internal_dict):
    if not re.match("\d+", command):
        print(f'Usage: day n')
        exit(1)

    os.makedirs(os.path.dirname(filename), exist_ok=True)
    with open(filename, "w") as f:
        f.write(f"{command}")

    compile(debugger, command, exe_ctx, result, internal_dict)

    debugger.HandleCommand(f"target create out/{command}")
    debugger.HandleCommand(f"settings set target.run-args day/{command}/input")


def compile(debugger, command, exe_ctx, result, internal_dict):
    try:
        with open(filename, 'r') as file:
            day = file.read()
    except Exception:
        print(f"Run 'day n' first")
        exit(1)
    result = subprocess.run(["clang", f"day/{day}/solution.c", "-g", "-o", f"out/{day}"], stdout=subprocess.PIPE)
