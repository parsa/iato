#!/usr/bin/env python3
"""
Run a command and treat “instruction break fault” exits as success.

Usage:
  expect_break.py --match "instruction break fault" -- <cmd> [args...]
"""

from __future__ import annotations

import argparse
import subprocess
import sys
from pathlib import Path


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--match",
        default="instruction break fault",
        help="Substring that, when present in stderr/stdout, downgrades failures",
    )
    parser.add_argument(
        "cmd",
        nargs=argparse.REMAINDER,
        help="Command to execute (after --)",
    )
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    if not args.cmd:
        print("expect_break.py: missing command to execute", file=sys.stderr)
        return 2

    # subprocess expects the command without the leading '--'
    if args.cmd[0] == "--":
        cmd = args.cmd[1:]
    else:
        cmd = args.cmd

    try:
        completed = subprocess.run(
            cmd,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True,
            check=False,
        )
    except OSError as exc:
        print(f"expect_break.py: failed to exec {cmd}: {exc}", file=sys.stderr)
        return 2

    output = completed.stdout or ""
    sys.stdout.write(output)
    sys.stdout.flush()

    if completed.returncode == 0:
        return 0
    if args.match and args.match in output:
        return 0
    return completed.returncode


if __name__ == "__main__":
    raise SystemExit(main())

