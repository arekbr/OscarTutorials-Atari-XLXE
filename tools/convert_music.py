#!/usr/bin/env python3
"""Sample the bundled PSID driver into 50 Hz, three-voice POKEY events.

Optional build-time dependency: py65. The XEX uses only the generated
notes and durations. The SID's sawtooth is played as POKEY pure tone.
"""

from collections import Counter
from pathlib import Path
import struct

from py65.devices.mpu6502 import MPU

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "Resources" / "gtu_a000_1x.sid"
DEST = ROOT / "atari" / "assets"


def register_trace():
    sid = SOURCE.read_bytes()
    if sid[:4] != b"PSID" or sid[18:22] != b"\0" * 4:
        raise ValueError("expected a vblank-driven PSID")
    offset, load, init, play = struct.unpack_from(">4H", sid, 6)
    if not load:
        load = int.from_bytes(sid[offset:offset + 2], "little")
        offset += 2
    cpu = MPU()
    end = load + len(sid) - offset
    cpu.memory[load:end] = sid[offset:]

    def call(address):
        cpu.sp = 255
        cpu.stPushWord(0x01FF)
        cpu.pc = address
        for _ in range(100_000):
            if cpu.pc == 0x0200:
                return
            cpu.step()
        raise ValueError(f"SID call ${address:04x} did not return")

    cpu.a = 0
    call(init)
    seen, trace = {}, []
    for _ in range(10_000):
        call(play)
        registers = bytes(cpu.memory[0xD400:0xD419])
        key = bytes(cpu.memory[load:end]) + bytes(cpu.memory[:0x100]) + registers
        if key in seen:
            return trace, seen[key]
        seen[key] = len(trace)
        trace.append(registers)
    raise ValueError("no repeat in SID state")


def convert(trace):
    divisors = []
    for registers in trace:
        frame = bytearray()
        master = registers[24] & 15
        for i in range(0, 21, 7):
            frequency = int.from_bytes(registers[i:i + 2], "little")
            flags = registers[i + 4]
            sustain = registers[i + 6] >> 4
            hz = frequency * 985248 / 16777216
            # The 15 kHz base clock gives a useful range for these three voices.
            divisor = 0 if not hz else min(255, max(0, round(1773447 / (228 * hz)) - 1))
            level = min(8, (2 + sustain) * master // 15)
            if not flags & 1 or flags & 8 or not frequency:
                level = 0
            frame.extend((divisor if level else 0, 0xa0 | level))
        divisors.append(bytes(frame))
    return divisors


def pack(frames):
    out = bytearray()
    position = 0
    while position < len(frames):
        frame = frames[position]
        duration = 1
        while (duration < 255 and position + duration < len(frames)
               and frames[position + duration] == frame):
            duration += 1
        out.extend(bytes([duration]) + frame)
        position += duration
    return bytes(out)


def main():
    trace, loop = register_trace()
    frames = convert(trace)
    (DEST / "music-intro.pokey").write_bytes(pack(frames[:loop]) + b"\0")
    (DEST / "music-loop.pokey").write_bytes(pack(frames[loop:]) + b"\0")
    print(f"{loop} intro frames and {len(frames) - loop} loop frames at 50 Hz")
    print(Counter(sum(value & 15 != 0 for value in frame[1::2]) for frame in frames))


if __name__ == "__main__":
    main()
