# OscarTutorials for Atari XL/XE

96 standalone examples for the Atari 800XL and 65XE with 64 KiB of RAM.
The port uses Oscar64 to build XEX executables. Each lesson in
`atari/NNNN/` matches the number of the original C64 lesson. The original
sources and resources are included for comparison.

Based on [OscarTutorials a8ec725](https://github.com/drmortalwombat/OscarTutorials/tree/a8ec7255f53c8040c875c2058d76128707b8ce31).
The original lessons and resources are by drmortalwombat and contributors.
Released under the [GNU General Public License v3.0](LICENSE).

The [Oscar64 Atari XL/XE library and samples](https://github.com/arekbr/oscar64-atari-include)
are available in a separate repository.

## Building and running

You need Oscar64 with its matching runtime headers, Python 3.8 or later,
and GNU Make. The tested compiler is [Oscar64 4f6df8b](https://github.com/drmortalwombat/oscar64/tree/4f6df8bd137d55c553673b38d181b5ae1fdf09a6).

```sh
make
make list
python3 tools/build.py 1300 1520 1750
```

`make` uses `build/toolchain/bin/oscar64` when present and looks for
`oscar64` on your PATH otherwise. Use `make OSCAR64=/path/to/oscar64`
or pass `--compiler` to the build script to select a different executable.
Builds are written to `build/`. Run a lesson in Atari800 with BASIC disabled.

```sh
atari800 -xl -pal -nobasic -run build/1750-VSpriteMuxIRQ.xex
```

On an Atari, load the XEX with BASIC disabled. The download uses short
names such as `1750.XEX`. Run `make list` to match the file numbers to
lesson titles. Use RESET to leave demos that keep running.

## Controls

| Lessons | Controls |
| --- | --- |
| 0030, 0300 | Press any key to stop the counter or generate the next maze, respectively. |
| 0050, 0070, 3000–3050 | Type with the keyboard and press RETURN. The adventure parser accepts commands such as `MOVE NORTH`, `LOOK`, or `INVENTORY`, depending on the lesson. |
| 0200 | Use Ctrl + Atari arrow keys or joystick 1. |
| 1160, 1260–1280, 1800–1820 | Use the Atari arrow keys, W/A/S/D, or joystick 1 to scroll the map. These controls change scrolling speed in 1280. |
| 1245, 1470–1480 | Use joystick 1 to scroll the map or move the ship in 1480. |
| 1600–1630 | Use joystick 1 to move the object. |
| 4200–4230, 4270–4290 | Use joystick 1 to move the crosshair for the angle and distance measurements. |

## How the lessons map to XL/XE hardware

| Lessons | Atari implementation |
| --- | --- |
| 0010–0110 | ATASCII, a 40×24 screen, direct screen memory access, and five vertical color bands in ANTIC mode 4. |
| 0400–1010 | Direct access to GTIA and ANTIC. VCOUNT advances once every two scanlines. |
| 1100–1160 | Use the Atari ROM character set or a custom font. CharPad resources are still loaded with `#embed`. |
| 1200–1280 | Copy characters and use LMS, HSCROL, and VSCROL. Faster versions split the copying into stages or rearrange the tile data. |
| 1300–1380, 1600–1630 | Use the four GTIA players. Atari versions of the C64 shapes use eight bits per row. Collisions come from GTIA or from testing the map against the object mask. |
| 1400–1480 | Use display list interrupts, independently scrolled bands, and background glyph changes. Lesson 1425 replaces the C128 clock switch with a timing comparison of the same loop with display DMA on and off. |
| 1500–1540 | Use ANTIC mode F and E bitmaps. Lesson 1520 changes the palette every eight lines, 1530 reserves part of the screen for text, and 1540 scrolls through 480 lines using successive LMS addresses. |
| 1700–1750 | Display list interrupts reposition the four GTIA players for each band. Lessons 1700 and 1710 show 16 objects, and 1720 and 1750 show 32. In 1750, a vertical blank interrupt switches to the prepared frame. |
| 1800–1820 | The ANTIC mode 4 palette approximates per-tile or per-character colors. Lesson 1820 draws the pixels into a four-color ANTIC mode E bitmap. |
| 2000–2010 | Three melodic lines from the original SID file play on POKEY. POKEY waveforms approximate the SID sawtooth. In 2000, the foreground calls the player once per frame. In 2010, a vertical blank interrupt calls it. |
| 4500–4530 | Demonstrate a 33 KiB contiguous heap, 46 KiB split across two RAM regions, resources in separate regions, and LZO overlays. |
| 5000–5030 | Animate a vector star at 320×192 using XOR, XOR with delayed erasure, double buffering with edge erasure, or double buffering with a full clear. |

Lessons 4000–4290 retain the original algorithms. The calculation timers
and music account for PAL and NTSC frame rates. Frame-based graphics
animations can run at different speeds in PAL and NTSC.

The ready-to-use Atari assets are included in `atari/assets/`. To
regenerate them, run `python3 tools/convert_bitmaps.py` and
`python3 tools/convert_music.py`. The music converter also needs `py65`.

See [verification results](PORTING.md#verification) for emulator
coverage and the hardware test status.
