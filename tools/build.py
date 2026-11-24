#!/usr/bin/env python3
"""Build the Atari counterpart of each numbered OscarTutorials directory."""

import argparse
import re
import shlex
import struct
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build"
LOCAL_COMPILER = BUILD / "toolchain" / "bin" / "oscar64"


def lessons():
    for original in sorted(ROOT.glob("[0-9][0-9][0-9][0-9] *")):
        number, name = original.name.split(" ", 1)
        folder = ROOT / "atari" / number
        sources = sorted(p for p in folder.glob("*") if p.suffix in (".c", ".cpp"))
        yield number, name, sources


def check_xex(target):
    data = target.read_bytes()
    if data[:2] != b"\xff\xff":
        raise ValueError("missing XEX signature")
    position = 2
    ranges = []
    run_address = None
    while position < len(data):
        if data[position:position + 2] == b"\xff\xff":
            position += 2
            continue
        if position + 4 > len(data):
            raise ValueError("incomplete segment header")
        start, end = struct.unpack_from("<HH", data, position)
        position += 4
        size = end - start + 1
        if size <= 0 or position + size > len(data):
            raise ValueError("invalid segment size")
        if start <= 0x02E0 and end >= 0x02E1:
            run_address = struct.unpack_from("<H", data, position + 0x02E0 - start)[0]
        elif not 0x2000 <= start <= end < 0xBC00:
            raise ValueError(f"segment ${start:04x}-${end:04x} outside program RAM")
        else:
            ranges.append((start, end))
        position += size
    if run_address != 0x2000 or not any(start <= run_address <= end for start, end in ranges):
        raise ValueError("missing Atari startup code")

    # Oscar64's XEX writer can coalesce overlapping sections into one
    # segment. Check the map as well, before accepting the executable.
    mapping = target.with_suffix(".map").read_text()
    code = re.search(r"(?m)^([0-9a-f]+) - [0-9a-f]+ : DATA, code$", mapping)
    if not code or int(code[1], 16) < 0x2080:
        raise ValueError("code overlaps the startup region $2000-$207f")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("ids", nargs="*", help="lesson numbers, default all")
    parser.add_argument("--compiler", default=str(LOCAL_COMPILER) if LOCAL_COMPILER.is_file() else "oscar64")
    parser.add_argument("--flags", default="-O2")
    parser.add_argument("--build-dir", type=Path, default=BUILD)
    parser.add_argument("--list", action="store_true")
    parser.add_argument("--clean", action="store_true")
    args = parser.parse_args()
    entries = list(lessons())
    unknown = set(args.ids) - {number for number, _, _ in entries}
    if unknown:
        parser.error("unknown lesson: " + ", ".join(sorted(unknown)))

    failures = []
    build_dir = args.build_dir.resolve()
    for number, name, sources in entries:
        if args.ids and number not in args.ids:
            continue
        if args.list:
            print(f"{number} {name}" + ("" if sources else " [no Atari source]"))
            continue
        target = build_dir / f"{number}-{re.sub(r'[^a-zA-Z0-9]+', '-', name)}.xex"
        if args.clean:
            for suffix in (".xex", ".asm", ".map", ".lbl", ".dbj", ".int"):
                target.with_suffix(suffix).unlink(missing_ok=True)
            continue
        if not sources:
            failures.append(f"{number}: no Atari source")
            continue
        build_dir.mkdir(parents=True, exist_ok=True)
        target.unlink(missing_ok=True)
        result = subprocess.run(
            [args.compiler, "-tm=atari", *shlex.split(args.flags), f"-o={target}",
             *(str(source) for source in sources)],
            cwd=ROOT,
        )
        if result.returncode or not target.is_file():
            failures.append(f"{number}: compilation failed")
        else:
            try:
                check_xex(target)
            except (OSError, ValueError) as error:
                target.unlink()
                failures.append(f"{number}: {error}")
            else:
                print(target, flush=True)
    if failures:
        print("\n".join(failures))
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
