# Ghost Executable `.GEX` design

Target: v1.3 design only. No loader is implemented in v1.2-alpha.

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
- Copy `image_size` bytes to `load_address`.
- Start execution at `load_address + entry_offset`.
- Keep execution in ring 0 for the first prototype.
- Return to `ghost32>` only through the `exit` syscall.

## Syscall Table

Initial table for tiny CLI programs:

```text
0  print   ESI = zero-terminated string
1  read    EDI = buffer, ECX = max bytes
2  exit    EAX = status
```

## Future Command

```text
ghost32> run HELLO.GEX
```

The v1.3 loader should stay read-only, FAT12-backed, and CLI-only. Networking,
TCP/IP, SOCKS5, Tor, graphics, audio, mouse, and multimedia remain out of scope.
