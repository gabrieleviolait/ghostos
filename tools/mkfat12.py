#!/usr/bin/env python3
"""Create a bootable Ghost OS 1.44 MB FAT12 floppy image.

The bootloader still loads the kernel from fixed hidden sectors. FAT12 starts
after those reserved sectors, and the same KERNEL.BIN is also copied as a
normal root-directory file for inspection.
"""

from __future__ import annotations

import math
import sys
from pathlib import Path


BYTES_PER_SECTOR = 512
TOTAL_SECTORS = 2880
SECTORS_PER_CLUSTER = 1
KERNEL_SECTORS = 177
RESERVED_SECTORS = 1 + KERNEL_SECTORS
FATS = 2
ROOT_ENTRIES = 224
SECTORS_PER_FAT = 9
ROOT_SECTORS = (ROOT_ENTRIES * 32 + BYTES_PER_SECTOR - 1) // BYTES_PER_SECTOR
FAT_START = RESERVED_SECTORS
ROOT_START = FAT_START + FATS * SECTORS_PER_FAT
DATA_START = ROOT_START + ROOT_SECTORS
MEDIA = 0xF0


def short_name(path: Path) -> bytes:
    name = path.name.upper()
    if name == "TORRELAYS.GHT":
        name = "TORRELAY.GHT"
    if "." in name:
        stem, ext = name.rsplit(".", 1)
    else:
        stem, ext = name, ""
    if not stem or len(stem) > 8 or len(ext) > 3:
        raise ValueError(f"{path.name!r} is not a FAT 8.3 name")
    allowed = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_$~!#%&-{}()@'`"
    if any(ch not in allowed for ch in stem + ext):
        raise ValueError(f"{path.name!r} contains unsupported FAT name chars")
    return stem.encode("ascii").ljust(8, b" ") + ext.encode("ascii").ljust(3, b" ")


def set_fat_entry(fat: bytearray, cluster: int, value: int) -> None:
    offset = cluster + cluster // 2
    value &= 0x0FFF
    if cluster & 1:
        fat[offset] = (fat[offset] & 0x0F) | ((value << 4) & 0xF0)
        fat[offset + 1] = (value >> 4) & 0xFF
    else:
        fat[offset] = value & 0xFF
        fat[offset + 1] = (fat[offset + 1] & 0xF0) | ((value >> 8) & 0x0F)


def add_file(
    image: bytearray,
    fat: bytearray,
    root: bytearray,
    root_index: int,
    next_cluster: int,
    path: Path,
    data: bytes,
) -> tuple[int, int]:
    clusters_needed = max(1, math.ceil(len(data) / BYTES_PER_SECTOR))
    first_cluster = next_cluster

    for i in range(clusters_needed):
        cluster = first_cluster + i
        next_value = 0xFFF if i == clusters_needed - 1 else cluster + 1
        set_fat_entry(fat, cluster, next_value)

        start = DATA_START * BYTES_PER_SECTOR + (cluster - 2) * BYTES_PER_SECTOR
        chunk = data[i * BYTES_PER_SECTOR : (i + 1) * BYTES_PER_SECTOR]
        image[start : start + len(chunk)] = chunk

    entry_offset = root_index * 32
    root[entry_offset : entry_offset + 11] = short_name(path)
    root[entry_offset + 11] = 0x20
    root[entry_offset + 26 : entry_offset + 28] = first_cluster.to_bytes(2, "little")
    root[entry_offset + 28 : entry_offset + 32] = len(data).to_bytes(4, "little")

    return root_index + 1, first_cluster + clusters_needed


def main(argv: list[str]) -> int:
    if len(argv) < 4:
        print("usage: mkfat12.py IMG BOOT.BIN KERNEL.BIN [FILES...]", file=sys.stderr)
        return 2

    img_path = Path(argv[1])
    boot_path = Path(argv[2])
    kernel_path = Path(argv[3])
    extra_paths = [Path(p) for p in argv[4:]]

    boot = boot_path.read_bytes()
    kernel = kernel_path.read_bytes()
    if len(boot) != BYTES_PER_SECTOR:
        raise SystemExit("boot.bin must be exactly 512 bytes")
    if len(kernel) > KERNEL_SECTORS * BYTES_PER_SECTOR:
        raise SystemExit(f"kernel.bin is larger than {KERNEL_SECTORS} sectors")

    image = bytearray(TOTAL_SECTORS * BYTES_PER_SECTOR)
    image[0:BYTES_PER_SECTOR] = boot
    image[BYTES_PER_SECTOR : BYTES_PER_SECTOR + len(kernel)] = kernel

    fat = bytearray(SECTORS_PER_FAT * BYTES_PER_SECTOR)
    fat[0:3] = bytes([MEDIA, 0xFF, 0xFF])
    root = bytearray(ROOT_SECTORS * BYTES_PER_SECTOR)

    root_index = 0
    next_cluster = 2
    root_index, next_cluster = add_file(
        image, fat, root, root_index, next_cluster, Path("KERNEL.BIN"), kernel
    )

    for path in extra_paths:
        root_index, next_cluster = add_file(
            image, fat, root, root_index, next_cluster, path, path.read_bytes()
        )

    for fat_number in range(FATS):
        start = (FAT_START + fat_number * SECTORS_PER_FAT) * BYTES_PER_SECTOR
        image[start : start + len(fat)] = fat

    root_start = ROOT_START * BYTES_PER_SECTOR
    image[root_start : root_start + len(root)] = root

    img_path.parent.mkdir(parents=True, exist_ok=True)
    img_path.write_bytes(image)
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
