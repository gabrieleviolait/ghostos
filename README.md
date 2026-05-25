# Ghost OS v0.5

Ghost OS is a tiny educational operating system written from scratch in pure NASM assembly.

It is intentionally simple: no Linux, no DOS, no GRUB, no existing kernel. The boot sector uses BIOS services, loads a small 16-bit kernel from raw disk sectors, and starts a CLI shell.

## Current target

- Architecture: x86
- Mode: 16-bit real mode
- Boot: BIOS boot sector
- Kernel: raw assembly binary loaded at `0000:1000`
- Interface: CLI only
- Test target: QEMU and VirtualBox

## Version map

### v0.1

- Valid 512-byte boot sector
- BIOS text output
- Boot signature `0xAA55`

### v0.2

- Bootloader loads a separate kernel from disk sectors
- Kernel starts at `0000:1000`

### v0.3

- Minimal CLI shell
- Prompt: `ghost>`
- Keyboard input with backspace
- Command parser

### v0.4

- Internal commands:
  - `help`
  - `clear`
  - `cls`
  - `echo TEXT`
  - `about`
  - `version`
  - `reboot`
  - `halt`

### v0.5

- Extra internal programs:
  - `calc 2+3`
  - `calc 8-5`
  - `mem`
  - `ascii`
  - `color N`
- Bootable floppy image
- Bootable ISO for VirtualBox
- Cleaner project identity and build checks

## Requirements

Linux/WSL recommended:

```bash
sudo apt update
sudo apt install nasm qemu-system-x86 xorriso make unzip
```

## Build

```bash
make clean
make
```

Outputs:

```text
build/ghostos.img
build/ghostos.iso
```

## Run in QEMU

Floppy image:

```bash
make run
```

ISO image:

```bash
make run-iso
```

## Run in VirtualBox

1. Create a new VM.
2. Type: `Other / Other 32-bit`.
3. RAM: 32 MB is enough.
4. Do not create a hard disk, unless you want one for later tests.
5. Mount `build/ghostos.iso` as optical disk.
6. Boot.

## Commands

```text
help
clear
cls
echo hello ghost
about
version
calc 2+3
calc 8-5
mem
ascii
color 2
reboot
halt
```

## Notes

This is still a real bootable OS experiment, but it is intentionally primitive. It uses BIOS interrupts and real mode, so it is not yet a modern protected-mode OS.

The next natural step is v0.6: FAT12 filesystem support, file listing, and loading small external `.BIN` programs from disk.
