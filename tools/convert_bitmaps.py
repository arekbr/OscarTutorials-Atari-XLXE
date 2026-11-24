#!/usr/bin/env python3
"""Convert the two C64 multicolour bitmaps to ANTIC E assets."""

from collections import Counter
from itertools import combinations
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
RESOURCES = ROOT / "Resources"
DEST = ROOT / "atari" / "assets"
RGB = (
    (0, 0, 0), (255, 255, 255), (136, 0, 0), (170, 255, 238),
    (204, 68, 204), (0, 204, 85), (0, 0, 170), (238, 238, 119),
    (221, 136, 85), (102, 68, 0), (255, 119, 119), (51, 51, 51),
    (119, 119, 119), (170, 255, 102), (0, 136, 255), (187, 187, 187),
)
GTIA = bytes([0, 14, 0x36, 0x9c, 0x68, 0xca, 0x84, 0x1e,
              0x28, 0x24, 0x4a, 0x04, 0x06, 0xce, 0x98, 0x0a])
DISTANCE = [
    [sum(w * (a - b) ** 2 for w, a, b in zip((2, 4, 1), src, dst))
     for dst in RGB]
    for src in RGB
]


def decode(name, height):
    source = (RESOURCES / name).read_bytes()
    num_cells = height * 5
    bitmap = source[:num_cells * 8]
    screen = source[num_cells * 8:num_cells * 9]
    color = source[num_cells * 9:]
    if len(color) != num_cells:
        raise ValueError(f"unexpected source size for {name}")
    rows = []
    for y in range(height):
        line = bytearray()
        for x in range(40):
            cell = y // 8 * 40 + x
            palette = 0, screen[cell] >> 4, screen[cell] & 15, color[cell] & 15
            pixels = bitmap[cell * 8 + y % 8]
            line.extend(palette[pixels >> shift & 3] for shift in (6, 4, 2, 0))
        rows.append(bytes(line))
    return rows


def choose_palette(rows):
    count = Counter(value for row in rows for value in row)
    candidates = [(0, *rest) for rest in combinations(range(1, len(RGB)), 3)]
    return min(candidates, key=lambda palette: sum(
        weight * min(DISTANCE[source][target] for target in palette)
        for source, weight in count.items()
    ))


def encode_rows(rows, colors):
    mapping = [min(range(4), key=lambda p: DISTANCE[source][colors[p]])
               for source in range(16)]
    for row in rows:
        yield bytes(mapping[row[x]] << 6 | mapping[row[x + 1]] << 4 |
                    mapping[row[x + 2]] << 2 | mapping[row[x + 3]]
                    for x in range(0, 160, 4))


def dma_blocks(rows):
    for offset in range(0, len(rows), 96):
        block = b"".join(rows[offset:offset + 96])
        yield block.ljust(4096, b"\0")


def main():
    photo = decode("blumba.mcimg", 200)
    photo = [photo[y * 200 // 192] for y in range(192)]
    palettes, photo_rows = [], []
    for first in range(0, 192, 8):
        band = photo[first:first + 8]
        colors = choose_palette(band)
        palettes.extend(GTIA[color] for color in colors)
        photo_rows.extend(encode_rows(band, colors))
    (DEST / "blumba.antic").write_bytes(b"".join(dma_blocks(photo_rows)))
    (DEST / "blumba.colors").write_bytes(bytes(palettes))

    landscape = decode("mchiresscroll.bin", 480)
    colors = choose_palette(landscape)
    converted = list(encode_rows(landscape, colors))
    (DEST / "bitmap-scroll.antic").write_bytes(b"".join(dma_blocks(converted)))
    (DEST / "bitmap-scroll.colors").write_bytes(bytes(GTIA[color] for color in colors))


if __name__ == "__main__":
    main()
