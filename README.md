# GhostOS

GhostOS is an experimental x86 operating system written primarily in Assembly, focused on understanding and implementing low-level networking, cryptography, and privacy-oriented protocols from first principles.

The project is developed as a learning and research platform to explore how modern network stacks and cryptographic systems operate beneath traditional operating systems and software frameworks.

---

## Status

| Area                        | Status         |
| --------------------------- | -------------- |
| Bootloader                  | ✅ Working      |
| Protected Mode              | ✅ Working      |
| Memory Foundations          | ✅ Working      |
| FAT12 Loader                | ✅ Working      |
| GEX Programs                | ✅ Working      |
| RTL8139 Driver              | ✅ Working      |
| ARP / IPv4 / ICMP           | ✅ Working      |
| UDP                         | ✅ Working      |
| TCP Client Flow             | ✅ Working      |
| SHA-256                     | ✅ Working      |
| HMAC-SHA256                 | ✅ Working      |
| HKDF-SHA256                 | ✅ Working      |
| RNG                         | ✅ Working      |
| Curve25519 Field Arithmetic | ✅ Working      |
| X25519                      | ✅ Working      |
| TLS Key Schedule Foundations | ✅ Experimental |
| TLS Record Parsing          | ✅ Experimental |
| GhostTor Relay Seed         | ✅ Experimental |
| Tor Link Protocol           | ⬜ Planned      |

---

## Current Features

### Core System

* 16-bit bootloader
* Protected Mode (`ghost32`)
* VGA text console
* Interactive shell
* Memory management foundations
* FAT12 filesystem image generation
* GEX executable loading

### Networking

* PCI device discovery
* Realtek RTL8139 driver
* Ethernet frame handling
* ARP protocol
* IPv4 packet processing
* ICMP echo requests and replies
* UDP packet transmission
* TCP SYN / SYN-ACK / ACK handshake
* TCP HTTP payload transmission
* TCP payload receive and ASCII dump
* Multi-segment TCP receive loop
* ACK for received TCP payload data
* TCP FIN receive and close handling
* Packet validation using Wireshark

### GhostTor Bootstrap

* Static relay seed file in the FAT12 image
* `torrelays` shell command
* `torpick N` relay selection command
* `torstate` selected relay state dump
* `torconn` TCP connect toward the selected relay
* Local fake relay seed for TCP testing on `10.0.2.2:9001`
* Minimal parser for relay lines and ntor keys
* Relay candidate count output
* Selected relay state storage for future GhostTor steps
* No Tor link cells, circuit creation, or anonymity yet

### Cryptography

#### GhostCrypto v0.1

* SHA-256 implementation
* Standard SHA-256 test vectors

#### GhostCrypto v0.2

* HMAC-SHA256
* ipad/opad processing
* Deterministic validation

#### GhostCrypto v0.3

* Entropy collection
* Pseudo-random number generation

#### GhostCrypto v0.4

##### Curve25519 / X25519

Implemented:

* Private scalar generation
* Private key clamping
* Curve25519 basepoint handling
* Field element buffers
* Field copy/add/subtract helpers
* Multiplication groundwork
* Carry propagation
* Reduction pipeline
* Real reduction modulo 2²⁵⁵−19
* Field squaring
* Field inversion chain
* Montgomery ladder initialization
* Conditional swap (cswap)
* Single ladder step implementation
* 255-bit ladder loop
* Public key generation
* Shared secret generation
* RFC7748 validation
* Deterministic testing infrastructure

#### GhostCrypto v0.5

##### HKDF-SHA256

Implemented:

* HKDF-Extract
* HKDF-Expand
* PRK derivation
* OKM derivation
* RFC5869 Test Case 1 validation
* Deterministic PRK / OKM verification

#### GhostTLS v0.1

##### Key Schedule Foundations

Implemented:

* TLS-style key schedule skeleton
* Early secret derivation
* Handshake secret derivation
* Client handshake traffic secret derivation
* Server handshake traffic secret derivation
* Client application traffic secret placeholder
* Server application traffic secret placeholder
* Deterministic key schedule validation
* `tlsscheduletest` shell command

---

## Development Status

GhostOS is currently in an experimental phase.

The networking stack is functional for low-level communication testing and now includes a minimal TCP client flow over the RTL8139 driver in QEMU user networking.

GhostNet currently supports ARP gateway discovery, ICMP ping testing, TCP connection establishment, HTTP `GET / HTTP/1.0` payload transmission, TCP payload receive/dump, multi-segment receive loops, ACKs for received payload data, and FIN close handling. This has been validated against a local Python HTTP server on `10.0.2.2:8080`.

The cryptographic subsystem now includes RFC7748-validated X25519, RFC5869-validated HKDF-SHA256, real GhostTLS X25519 key exchange, transcript hash construction, TLS 1.3 handshake secret derivation, traffic secret derivation, selected cipher reporting, and a TLS record parser capable of extracting ServerHello extensions. The current TLS path has negotiated ChaCha20-Poly1305 with `selected_cipher=0x1303`; the next block is handshake key/IV derivation and ChaCha20-Poly1305 record protection groundwork.

GhostTor has started as a separate native MVP track. The current stage adds a static relay seed file (`TORRELAYS.GHT`), a `torrelays` command that reads relay candidates and ntor keys from the FAT12 image, a `torpick N` relay selection command, a `torstate` diagnostic state dump, and a `torconn` TCP-connect wrapper toward the selected relay IP/ORPort. This is bootstrap/debug infrastructure only, not an anonymous Tor client.

---

## RFC References

The X25519 implementation has been validated against the official test vectors defined in RFC 7748:

* RFC 7748 — Elliptic Curves for Security
* Section 5.2 Test Vectors:
  https://datatracker.ietf.org/doc/html/rfc7748#section-5.2

The HKDF-SHA256 implementation has been validated against RFC 5869 Test Case 1:

* RFC 5869 — HMAC-based Extract-and-Expand Key Derivation Function
* Test Case 1:
  https://datatracker.ietf.org/doc/html/rfc5869#appendix-A.1

GhostOS now matches the RFC7748 X25519 public key/shared secret reference vectors and the RFC5869 HKDF-SHA256 PRK/OKM reference vectors.

---

## Roadmap

### Current GhostTLS Status

#### GhostNet / TCP

* ✅ RTL8139 init
* ✅ ARP gateway
* ✅ ICMP ping
* ✅ TCP SYN / SYN-ACK / ACK
* ✅ TCP ESTABLISHED
* ✅ TCP payload send
* ✅ TCP payload receive
* ✅ ACK for received payload data
* ✅ Automated TLS test with `tlstest`
* ✅ Interactive `-- press key --` checkpoints

#### GhostCrypto

* ✅ SHA-256
* ✅ HMAC-SHA256
* ✅ HKDF-SHA256 RFC5869
* ✅ Curve25519 / X25519 RFC7748
* ✅ X25519 shared secret test PASS

#### GhostTLS v0.2 / v0.3

* ✅ TLS 1.3 ClientHello
* ✅ X25519 `key_share` in ClientHello
* ✅ ServerHello received
* ✅ TLS record parser
* ✅ Multi-record parser
* ✅ ServerHello field extraction
* ✅ `selected_version = 0x0304`
* ✅ Server `key_share` parsed
* ✅ `server_pubkey saved`

#### GhostTLS v0.4

* ✅ Runtime-generated real X25519 client `key_share`
* ✅ Server X25519 public key saved
* ✅ Real X25519 shared secret derived

#### GhostTLS v0.5

* ✅ Transcript hash built
* ✅ Handshake secret derived
* ✅ Client handshake traffic secret derived
* ✅ Server handshake traffic secret derived
* ✅ `tlssecrets` command
* ✅ Selected cipher summary
* ✅ ChaCha20-Poly1305 negotiated: `selected_cipher=0x1303`

### Next Block: GhostTLS v0.6

GhostTLS now follows the ChaCha20-Poly1305 path rather than AES-GCM.

#### v0.6a — Derive Handshake Key/IV

Derive:

```text
server_hs_key = HKDF-Expand-Label(server_hs_traffic_secret, "key", "", 32)
server_hs_iv  = HKDF-Expand-Label(server_hs_traffic_secret, "iv", "", 12)
client_hs_key = HKDF-Expand-Label(client_hs_traffic_secret, "key", "", 32)
client_hs_iv  = HKDF-Expand-Label(client_hs_traffic_secret, "iv", "", 12)
```

Output target:

```text
server_hs_key derived
server_hs_iv derived
client_hs_key derived
client_hs_iv derived
```

#### v0.6b — ChaCha20 Block Function

Implement and test:

* Quarter round
* 20 rounds
* State serialization
* Counter
* 96-bit nonce
* RFC/standard test vector before TLS integration

Output target:

```text
chacha20 block PASS
```

#### v0.6c — Poly1305

Implement and test:

* One-time key from ChaCha20 block counter 0
* Poly1305 MAC
* Tag verification

Output target:

```text
poly1305 PASS
```

#### v0.6d — ChaCha20-Poly1305 AEAD Decrypt

Implement:

* AAD = TLS record header
* Ciphertext = encrypted record payload without tag
* Tag = final 16 bytes
* Nonce = static IV XOR sequence number

Output target:

```text
handshake record decrypted
tag verified
```

#### v0.6e — Read EncryptedExtensions

After AEAD decrypt works, parse the first encrypted TLS 1.3 handshake messages:

* EncryptedExtensions
* Certificate
* CertificateVerify
* Finished

Minimum output target:

```text
decrypted_type=Handshake
handshake_name=EncryptedExtensions
```

### After v0.6

#### GhostTLS v0.7 — Finished Verification

* Progressive transcript hash update
* Server Finished `verify_data`
* Client Finished construction
* Encrypted client Finished record send

#### GhostTLS v0.8 — Application Traffic

* Derive client/server application traffic secrets
* Derive application key/IV
* Send encrypted HTTP GET
* Receive encrypted HTTPS response
* Decrypt HTTP response

Dream output:

```text
tlsget 10.0.2.2 4433 /
HTTP/1.1 200 OK
...
```

### GhostTor Native MVP

#### GhostTor v0.1

* ✅ Static relay seed file: `pages/TORRELAYS.GHT`
* ✅ FAT12 8.3 image alias: `TORRELAY.GHT`
* ✅ `torrelays` shell command
* ✅ `torpick 1/2/3` relay selection command
* ✅ Selected relay state storage
* ✅ `torstate` diagnostic command
* ✅ `torconn` TCP connect toward selected relay IP/ORPort
* ✅ Local fake relay seed: `localor 10.0.2.2:9001`
* ✅ Minimal parser for `Relay ...` and `ntor=...` lines
* ✅ Relay candidate count output

#### GhostTor v0.2

* Directory HTTP client experiment
* `torconsensus` command
* Download current consensus over plain HTTP
* Minimal consensus parsing

#### GhostTor v0.3

* TLS toward a Tor relay
* VERSIONS cell
* NETINFO cell

#### GhostTor v0.4

* CREATE2 / CREATED2
* ntor handshake
* Circuit keys

#### GhostTor v0.5

* RELAY_BEGIN
* RELAY_DATA
* RELAY_END
* HTTP request through an exit node

#### GhostTor v0.6+

* Multi-hop circuits
* Directory consensus / relay selection
* Onion services later

---

## Building

Requirements:

* NASM
* Python 3
* QEMU

Build image:

```bash
make img
```

Run:

```bash
make run
```

Or manually:

```bash
nasm -f bin boot/boot.asm -o build/boot.bin
nasm -f bin -w-label-redef-late kernel/kernel.asm -o build/kernel.bin
python tools/mkfat12.py build/ghostos.img build/boot.bin build/kernel.bin
```

---

## Example Commands

```text
help

pingtest
udpsend 10.0.2.2 1234
tcpconnect 10.0.2.2 80
tcpstat

sha256test
hmacsha256test
hkdfsha256test
tlsscheduletest
tlstest
tlssecrets
rngtest
rfc7748test
torrelays
torpick 1
torstate
torconn
```

---

## Security Notice

GhostOS is an experimental educational project.

The networking and cryptographic implementations are under active development and must not be considered production-ready or security-audited.

Do not use GhostOS to protect sensitive information, secure communications, or production systems.

---

## License

GhostOS is licensed under the GNU Affero General Public License v3.0 or later (AGPLv3).

---

## Disclaimer

This project is experimental and intended for educational and research purposes only.

It is not production-ready and should not be used in environments where security, privacy, reliability, or legal compliance are required.

The author is not responsible for misuse, damage, data loss, security incidents, or any consequences resulting from the use of this software.

Use at your own risk.

GhostOS does not currently guarantee confidentiality, authenticity, forward secrecy, anonymity, resistance to traffic analysis, or protection against active network attackers.
