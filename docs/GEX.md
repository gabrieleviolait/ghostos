# Ghost Executable `.GEX` design

Target: v1.3-alpha. The first protected-mode loader is implemented for
small ring-0 CLI programs.

## Header

All fields are little-endian.

```text
offset  size  field
0       4     magic = "GEX1"
4       4     entry_offset
8       4     load_address
12      4     image_size
16      4     flags
20      12    reserved, zero
```

The executable image starts immediately after the 32-byte header.

## Initial Loader Rules

- Reject files without `GEX1`.
- Reject images larger than the available load region.
- Reject non-zero flags in v1.3-alpha.
- Reject load addresses other than `0x00180000`.
- Reject entry offsets outside the image.
- Copy `image_size` bytes to `load_address`.
- Start execution at `load_address + entry_offset`.
- Keep execution in ring 0 for the first prototype.
- Return to `ghost32>` through the `exit` syscall. A raw `ret` from the
  entry point is also treated as a clean return for loader safety.

## Syscall Table

The loader passes the syscall entry point in `EDX`. Programs set `EAX` to
the syscall number and use the registers below.

```text
0  print   ESI = zero-terminated string
1  read    EDI = buffer, ECX = max bytes, returns EAX = bytes read
2  exit    EBX = status
```

## Command

```text
ghost32> run HELLO.GEX
```

The v1.3 loader should stay read-only, FAT12-backed, and CLI-only. Networking,
TCP/IP, SOCKS5, Tor, graphics, audio, mouse, and multimedia remain out of scope.
