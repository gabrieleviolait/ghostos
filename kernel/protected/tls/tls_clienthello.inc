; ============================================================
; GhostTLS - Minimal TLS plaintext ClientHello record
; GhostTLS v0.2a diagnostic payload
; ============================================================

; TLSPlaintext record:
;   ContentType: handshake (0x16)
;   legacy_record_version: 0x0301
;   length: 54
;
; Handshake:
;   msg_type: client_hello (0x01)
;   length: 50
;
; ClientHello:
;   legacy_version: 0x0303
;   deterministic 32-byte random
;   empty session_id
;   cipher_suites: TLS_AES_128_GCM_SHA256 (0x1301)
;   compression_methods: null
;   extensions:
;     supported_versions: TLS 1.3 (0x0304)

tls_clienthello_payload:
    db 016h, 003h, 001h, 000h, 036h

    db 001h, 000h, 000h, 032h

    db 003h, 003h

    db 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h
    db 008h, 009h, 00Ah, 00Bh, 00Ch, 00Dh, 00Eh, 00Fh
    db 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h
    db 018h, 019h, 01Ah, 01Bh, 01Ch, 01Dh, 01Eh, 01Fh

    db 000h

    db 000h, 002h
    db 013h, 001h

    db 001h
    db 000h

    db 000h, 007h
    db 000h, 02Bh, 000h, 003h
    db 002h, 003h, 004h

tls_clienthello_len equ $ - tls_clienthello_payload
