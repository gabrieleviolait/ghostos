# GhostOS

GhostOS is an experimental x86 operating system written primarily in Assembly, focused on understanding and implementing low-level networking, cryptography, and privacy-oriented protocols from first principles.

The project is developed as a learning and research platform to explore how modern network stacks and cryptographic systems operate beneath traditional operating systems and software frameworks.


=======
## Status

| Area | Status |
|---|---|
| Bootloader | ✅ Working |
| Protected Mode | ✅ Working |
| RTL8139 Driver | ✅ Working |
| ARP / IPv4 / ICMP | ✅ Working |
| UDP | ✅ Working |
| TCP Handshake | ✅ Working |
| SHA-256 | ✅ Working |
| HMAC-SHA256 | ✅ Working |
| RNG | ✅ Basic |
| Curve25519 / X25519 | 🚧 Work in Progress |
| TLS | ⬜ Planned |
| Tor Link Protocol | ⬜ Planned |

## Current Features

### Core System

* 16-bit bootloader
* Protected Mode (ghost32)
* Memory management foundations
* Interactive shell
* VGA text interface

### Networking

* PCI device discovery
* Realtek RTL8139 network driver
* Ethernet frame handling
* ARP protocol
* IPv4 packet processing
* ICMP echo requests and replies
* UDP packet transmission
* Minimal TCP handshake implementation

### Cryptography

#### GhostCrypto v0.1

* SHA-256 implementation
* SHA-256 test vectors

#### GhostCrypto v0.2

* HMAC-SHA256
* ipad/opad processing
* Deterministic test validation

#### GhostCrypto v0.3

* Basic entropy collection
* Pseudo-random number generation

#### GhostCrypto v0.4 (Work In Progress)

* Curve25519 foundations
* X25519 scalar preparation
* Private key clamping
* Basepoint handling
* Field arithmetic experiments
* Early multiplication and reduction pipeline

## Development Status

GhostOS is currently in an experimental phase.

The networking stack is functional for basic communication and testing.

The cryptographic subsystem is under active development, with the long-term goal of supporting modern privacy-preserving protocols.

## Roadmap

### Curve25519 / X25519
The X25519 implementation is being developed against the reference test vectors from [RFC 7748, Section 5.2](https://datatracker.ietf.org/doc/html/rfc7748#section-5.2).

* Field arithmetic refinement
* Proper reduction modulo 2^255−19
* Montgomery ladder
* X25519 scalar multiplication
* Shared secret generation

### GhostTLS

* TLS record layer
* ClientHello generation
* Key derivation
* Encrypted records

### GhostTor

* Tor link protocol
* VERSIONS and NETINFO cells
* CREATE2 / CREATED2
* Circuit establishment
* Minimal relay support

## Building

Requirements:

* NASM
* Python 3
* QEMU

Example build process:

```bash
nasm -f bin boot/boot.asm -o build/boot.bin
nasm -f bin -w-label-redef-late kernel/kernel.asm -o build/kernel.bin
python tools/mkfat12.py build/ghostos.img build/boot.bin build/kernel.bin
```

Run:

```bash
qemu-system-i386 \
  -drive format=raw,file=build/ghostos.img \
  -device rtl8139,netdev=n0 \
  -netdev user,id=n0
```

## Example Commands

```text
help
pingtest
udpsend 10.0.2.2 1234
tcpconnect 10.0.2.2 80
tcpstat

sha256test
hmacsha256test
rngtest
curve25519test
```

## Security Notice

GhostOS is an experimental educational project.

The networking and cryptographic implementations are under active development and must not be considered production-ready or security-audited.

Do not use GhostOS to protect sensitive information.

## License

GhostOS is licensed under the GNU Affero General Public License v3.0 or later (AGPLv3).

## Disclaimer

This project is experimental and intended for educational and research purposes only.

It is not production-ready and should not be used in environments where security, privacy, reliability, or legal compliance are required.

The author is not responsible for misuse, damage, data loss, security incidents, or any consequences resulting from the use of this software.

Use at your own risk.

