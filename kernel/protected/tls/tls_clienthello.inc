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

tls_clienthello_keyshare_public:
    db 085h, 020h, 0F0h, 009h, 089h, 030h, 0A7h, 054h
    db 074h, 08Bh, 07Dh, 0DCh, 0B4h, 03Eh, 0F7h, 05Ah
    db 00Dh, 0BFh, 03Ah, 00Dh, 026h, 038h, 01Ah, 0F4h
    db 0EBh, 0A4h, 0A9h, 08Eh, 0AAh, 09Bh, 04Eh, 06Ah

tls_clienthello_len equ $ - tls_clienthello_payload

; GhostTLS v0.4a deterministic X25519 client key_share.
; This is a real X25519 public key generated at runtime from this
; deterministic private key. RNG-based ephemeral generation comes next.
tls_client_private_key:
    db 077h, 007h, 06Dh, 00Ah, 073h, 018h, 0A5h, 07Dh
    db 03Ch, 016h, 0C1h, 072h, 051h, 0B2h, 066h, 045h
    db 0DFh, 04Ch, 02Fh, 087h, 0EBh, 0C0h, 099h, 02Ah
    db 0B1h, 077h, 0FBh, 0A5h, 01Dh, 0B9h, 02Ch, 02Ah

tls_client_keyshare_generated_msg db 'client key_share generated',13,0
tls_shared_secret_derived_msg db 'shared_secret derived',13,0

tls_shared_secret_ready db 0
tls_shared_secret times 32 db 0

tls_clienthello_prepare_keyshare:
    pushad

    ; curve25519_private_key = deterministic client private key
    mov esi, tls_client_private_key
    mov edi, curve25519_private_key
    call curve25519_fe_copy

    ; Clamp private scalar for X25519
    call curve25519_clamp_private

    ; public = X25519(private, basepoint)
    call x25519_ladder_init
    call x25519_generate_public_skeleton

    ; Copy generated public key into ClientHello key_share bytes
    mov esi, curve25519_public_key
    mov edi, tls_clienthello_keyshare_public
    call curve25519_fe_copy

    mov esi, tls_client_keyshare_generated_msg
    call pm_print_string

    popad
    ret

tls_derive_shared_secret:
    pushad

    ; curve25519_private_key = deterministic client private key
    mov esi, tls_client_private_key
    mov edi, curve25519_private_key
    call curve25519_fe_copy

    ; Clamp private scalar for X25519
    call curve25519_clamp_private

    ; peer public key = ServerHello key_share
    mov esi, rtl8139_tls_server_pubkey
    mov edi, x25519_peer_public_key
    call curve25519_fe_copy

    ; shared_secret = X25519(client_private, server_public)
    call x25519_generate_shared_secret_skeleton

    mov esi, x25519_shared_secret
    mov edi, tls_shared_secret
    call curve25519_fe_copy

    mov byte [tls_shared_secret_ready], 1

    mov esi, tls_shared_secret_derived_msg
    call pm_print_string

    popad
    ret
