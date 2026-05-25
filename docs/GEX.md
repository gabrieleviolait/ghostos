# Ghost Executable `.GEX` design

Target: v1.4.5-alpha. The protected-mode loader supports small ring-0
CLI programs with a stable syscall ABI and runtime metadata.

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
- Reject non-zero flags in v1.4.5-alpha.
- Reject load addresses other than `0x00180000`.
- Reject entry offsets outside the image.
- Copy `image_size` bytes to `load_address`.
- Start execution at `load_address + entry_offset`.
- Keep execution in ring 0 for the first prototype.
- Return to `ghost32>` through the `exit` syscall. A raw `ret` from the
  entry point is also treated as a clean return for loader safety.

## Runtime Metadata

The loader passes:

```text
EAX = argc
EBX = argv pointer
ECX = metadata pointer
EBP = metadata pointer
EDX = legacy v1.3 syscall shim
```

The metadata struct is:

```text
offset  size  field
0       4     magic = "GEXM"
4       4     abi_version = 0x00010405
8       4     argc
12      4     argv pointer
16      4     stable syscall dispatcher pointer
20      4     version string pointer
```

`argv[0]` is the program filename and later entries are arguments from the
`run` command.

## Syscall ABI

Stable v1.4.5 calls use the dispatcher pointer from metadata. The register ABI is:

```text
EDX = syscall number
ESI = arg1
EDI = arg2
ECX = arg length / optional size
EAX = return value
```

Syscalls:

```text
0  print        ESI = zero-terminated string, returns EAX = 0
1  read         EDI = buffer, ECX = max bytes, returns EAX = bytes read
2  exit         ESI = status, returns to ghost32>
3  get_version  returns EAX = zero-terminated version string pointer
4  get_ticks    returns EAX = PIT/system ticks
```

The v1.3 `call edx` convention remains available as a compatibility shim for
existing `.GEX` programs: `EAX` holds the syscall number and `EBX` holds the
exit status for `exit`.

## Command

```text
ghost32> run HELLO.GEX
ghost32> run HELLO.GEX hello world
```

The loader remains read-only, FAT12-backed, and CLI-only. Networking, TCP/IP,
SOCKS5, Tor, graphics, audio, mouse, and multimedia remain out of scope.
