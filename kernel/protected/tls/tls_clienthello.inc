; ============================================================
; GhostTLS - Minimal TLS 1.3 ClientHello record
; GhostTLS v0.2b compatibility payload
; ============================================================

; TLSPlaintext:
;   type    = 0x16 handshake
;   version = 0x0301 legacy record version
;   length  = 0x0072
;
; Handshake:
;   type   = 0x01 ClientHello
;   length = 0x00006e
;
; ClientHello body length:
;   fixed clienthello fields = 43 bytes
;   extensions              = 67 bytes
;   total                   = 110 bytes

tls_clienthello_payload:
    ; TLS record header
    db 016h, 003h, 001h, 000h, 072h

    ; Handshake header: ClientHello, length 110
    db 001h, 000h, 000h, 06Eh

    ; legacy_version TLS 1.2
    db 003h, 003h

    ; deterministic 32-byte random for diagnostic tests
    db 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h
    db 008h, 009h, 00Ah, 00Bh, 00Ch, 00Dh, 00Eh, 00Fh
    db 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h
    db 018h, 019h, 01Ah, 01Bh, 01Ch, 01Dh, 01Eh, 01Fh

    ; legacy_session_id length = 0
    db 000h

    ; cipher_suites length = 2
    ; TLS_AES_128_GCM_SHA256 = 0x1301
    db 000h, 002h
    db 013h, 001h

    ; compression_methods length = 1, null compression
    db 001h
    db 000h

    ; extensions length = 67
    db 000h, 043h

    ; extension: supported_versions
    ; type 0x002b, len 3, list len 2, TLS 1.3 = 0x0304
    db 000h, 02Bh, 000h, 003h
    db 002h, 003h, 004h

    ; extension: supported_groups
    ; type 0x000a, len 4, list len 2, x25519 = 0x001d
    db 000h, 00Ah, 000h, 004h
    db 000h, 002h, 000h, 01Dh

    ; extension: signature_algorithms
    ; type 0x000d, len 6, list len 4
    ; rsa_pss_rsae_sha256 = 0x0804
    ; ecdsa_secp256r1_sha256 = 0x0403
    db 000h, 00Dh, 000h, 006h
    db 000h, 004h
    db 008h, 004h
    db 004h, 003h

    ; extension: key_share
    ; type 0x0033, len 38
    ; client_shares vector len 36
    ; group x25519, key_exchange len 32
    ; static RFC7748-style diagnostic X25519 public key
    db 000h, 033h, 000h, 026h
    db 000h, 024h
    db 000h, 01Dh
    db 000h, 020h
    db 085h, 020h, 0F0h, 009h, 089h, 030h, 0A7h, 054h
    db 074h, 08Bh, 07Dh, 0DCh, 0B4h, 03Eh, 0F7h, 05Ah
    db 00Dh, 0BFh, 03Ah, 00Dh, 026h, 038h, 01Ah, 0F4h
    db 0EBh, 0A4h, 0A9h, 08Eh, 0AAh, 09Bh, 04Eh, 06Ah

tls_clienthello_len equ $ - tls_clienthello_payload
