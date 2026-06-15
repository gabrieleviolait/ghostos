; ============================================================
; GhostTLS - TLS 1.3 Key Schedule Foundations
; v0.1 structural test
; ============================================================

tls_key_schedule_test:
    pushad

    mov byte [tlsks_fail_flag], 0

    ; early_secret = HMAC(zero_salt, zero_secret)
    mov esi, tlsks_zero_salt
    mov ecx, HKDF_HASH_SIZE
    mov edx, tlsks_zero_secret
    mov ebx, HKDF_HASH_SIZE
    mov edi, tlsks_early_secret
    call hmac_sha256_hash

    mov esi, tlsks_early_secret
    mov edi, tlsks_expected_early
    mov ecx, HKDF_HASH_SIZE
    call tlsks_memcmp
    mov esi, tlsks_early_msg
    call tlsks_print_result

    ; handshake_secret = HMAC(early_secret, deterministic shared secret)
    mov esi, tlsks_early_secret
    mov ecx, HKDF_HASH_SIZE
    mov edx, tlsks_shared_secret
    mov ebx, HKDF_HASH_SIZE
    mov edi, tlsks_handshake_secret
    call hmac_sha256_hash

    mov esi, tlsks_handshake_secret
    mov edi, tlsks_expected_handshake
    mov ecx, HKDF_HASH_SIZE
    call tlsks_memcmp
    mov esi, tlsks_handshake_msg
    call tlsks_print_result

    ; client handshake traffic secret
    mov esi, tlsks_chs_label
    mov ecx, tlsks_chs_label_len
    call tlsks_build_label_hash
    mov esi, tlsks_handshake_secret
    mov ecx, HKDF_HASH_SIZE
    mov edx, tlsks_input
    mov ebx, tlsks_chs_input_len
    mov edi, tlsks_chs
    call hmac_sha256_hash

    mov esi, tlsks_chs
    mov edi, tlsks_expected_chs
    mov ecx, HKDF_HASH_SIZE
    call tlsks_memcmp
    mov esi, tlsks_chs_msg
    call tlsks_print_result

    ; server handshake traffic secret
    mov esi, tlsks_shs_label
    mov ecx, tlsks_shs_label_len
    call tlsks_build_label_hash
    mov esi, tlsks_handshake_secret
    mov ecx, HKDF_HASH_SIZE
    mov edx, tlsks_input
    mov ebx, tlsks_shs_input_len
    mov edi, tlsks_shs
    call hmac_sha256_hash

    mov esi, tlsks_shs
    mov edi, tlsks_expected_shs
    mov ecx, HKDF_HASH_SIZE
    call tlsks_memcmp
    mov esi, tlsks_shs_msg
    call tlsks_print_result

    ; client application traffic secret
    mov esi, tlsks_cats_label
    mov ecx, tlsks_cats_label_len
    call tlsks_build_label_hash
    mov esi, tlsks_handshake_secret
    mov ecx, HKDF_HASH_SIZE
    mov edx, tlsks_input
    mov ebx, tlsks_cats_input_len
    mov edi, tlsks_cats
    call hmac_sha256_hash

    mov esi, tlsks_cats
    mov edi, tlsks_expected_cats
    mov ecx, HKDF_HASH_SIZE
    call tlsks_memcmp
    mov esi, tlsks_cats_msg
    call tlsks_print_result

    ; server application traffic secret
    mov esi, tlsks_sats_label
    mov ecx, tlsks_sats_label_len
    call tlsks_build_label_hash
    mov esi, tlsks_handshake_secret
    mov ecx, HKDF_HASH_SIZE
    mov edx, tlsks_input
    mov ebx, tlsks_sats_input_len
    mov edi, tlsks_sats
    call hmac_sha256_hash

    mov esi, tlsks_sats
    mov edi, tlsks_expected_sats
    mov ecx, HKDF_HASH_SIZE
    call tlsks_memcmp
    mov esi, tlsks_sats_msg
    call tlsks_print_result

    mov eax, 1
    cmp byte [tlsks_fail_flag], 0
    je .print_final
    xor eax, eax

.print_final:
    mov esi, tlsks_final_msg
    call tlsks_print_result

    popad
    ret


tlsks_build_label_hash:
    ; ESI = label
    ; ECX = label length
    pushad

    mov edi, tlsks_input

.copy_label:
    cmp ecx, 0
    je .copy_hash_start
    lodsb
    stosb
    dec ecx
    jmp .copy_label

.copy_hash_start:
    mov esi, tlsks_transcript_hash
    mov ecx, HKDF_HASH_SIZE

.copy_hash:
    lodsb
    stosb
    loop .copy_hash

    popad
    ret


tlsks_memcmp:
    ; ESI = actual
    ; EDI = expected
    ; ECX = length
    ; EAX = 1 equal, 0 different
    push esi
    push edi
    push ecx

.compare:
    cmp ecx, 0
    je .equal

    mov al, [esi]
    cmp al, [edi]
    jne .different

    inc esi
    inc edi
    dec ecx
    jmp .compare

.equal:
    mov eax, 1
    jmp .done

.different:
    xor eax, eax

.done:
    pop ecx
    pop edi
    pop esi
    ret


tlsks_print_result:
    ; ESI = label
    ; EAX = 1 pass, 0 fail
    push eax

    call pm_print_string

    pop eax
    cmp eax, 1
    je .pass

    mov byte [tlsks_fail_flag], 1
    mov esi, tlsks_fail_msg
    call pm_print_string
    ret

.pass:
    mov esi, tlsks_pass_msg
    call pm_print_string
    ret


; ============================================================
; Messages
; ============================================================

tlsks_early_msg db 'EARLY ',0
tlsks_handshake_msg db 'HANDSHAKE ',0
tlsks_chs_msg db 'CHS ',0
tlsks_shs_msg db 'SHS ',0
tlsks_cats_msg db 'CATS ',0
tlsks_sats_msg db 'SATS ',0
tlsks_final_msg db 'TLSKS ',0
tlsks_pass_msg db 'PASS',13,0
tlsks_fail_msg db 'FAIL',13,0

; ============================================================
; Deterministic TLS key schedule seed material
; ============================================================

tlsks_zero_salt times HKDF_HASH_SIZE db 0
tlsks_zero_secret times HKDF_HASH_SIZE db 0

tlsks_shared_secret:
    db 011h,012h,013h,014h,015h,016h,017h,018h
    db 019h,01Ah,01Bh,01Ch,01Dh,01Eh,01Fh,020h
    db 021h,022h,023h,024h,025h,026h,027h,028h
    db 029h,02Ah,02Bh,02Ch,02Dh,02Eh,02Fh,030h

tlsks_transcript_hash:
    db 0A0h,0A1h,0A2h,0A3h,0A4h,0A5h,0A6h,0A7h
    db 0A8h,0A9h,0AAh,0ABh,0ACh,0ADh,0AEh,0AFh
    db 0B0h,0B1h,0B2h,0B3h,0B4h,0B5h,0B6h,0B7h
    db 0B8h,0B9h,0BAh,0BBh,0BCh,0BDh,0BEh,0BFh

tlsks_chs_label db 'c hs traffic'
tlsks_chs_label_len equ $ - tlsks_chs_label
tlsks_shs_label db 's hs traffic'
tlsks_shs_label_len equ $ - tlsks_shs_label
tlsks_cats_label db 'c ap traffic'
tlsks_cats_label_len equ $ - tlsks_cats_label
tlsks_sats_label db 's ap traffic'
tlsks_sats_label_len equ $ - tlsks_sats_label

tlsks_chs_input_len equ tlsks_chs_label_len + HKDF_HASH_SIZE
tlsks_shs_input_len equ tlsks_shs_label_len + HKDF_HASH_SIZE
tlsks_cats_input_len equ tlsks_cats_label_len + HKDF_HASH_SIZE
tlsks_sats_input_len equ tlsks_sats_label_len + HKDF_HASH_SIZE

; ============================================================
; Expected deterministic outputs
; ============================================================

tlsks_expected_early:
    db 033h, 0ADh, 00Ah, 01Ch, 060h, 07Eh, 0C0h, 03Bh
    db 009h, 0E6h, 0CDh, 098h, 093h, 068h, 00Ch, 0E2h
    db 010h, 0ADh, 0F3h, 000h, 0AAh, 01Fh, 026h, 060h
    db 0E1h, 0B2h, 02Eh, 010h, 0F1h, 070h, 0F9h, 02Ah

tlsks_expected_handshake:
    db 0EDh, 07Ch, 01Eh, 0C4h, 069h, 0DBh, 079h, 026h
    db 0C7h, 0FFh, 039h, 0C1h, 06Ch, 02Eh, 01Ch, 0A9h
    db 023h, 069h, 0ECh, 056h, 05Eh, 038h, 088h, 061h
    db 0F4h, 036h, 0E4h, 099h, 006h, 0CBh, 0ABh, 040h

tlsks_expected_chs:
    db 0D2h, 0B0h, 0F4h, 094h, 029h, 032h, 04Fh, 058h
    db 090h, 0A1h, 015h, 02Ch, 02Ah, 065h, 025h, 0A4h
    db 0CEh, 01Ah, 0EFh, 0B4h, 0CFh, 0E2h, 01Ch, 01Fh
    db 0B8h, 05Eh, 04Dh, 058h, 003h, 0C6h, 0C2h, 04Fh

tlsks_expected_shs:
    db 0E3h, 0ABh, 0B9h, 08Ch, 0F1h, 017h, 0CDh, 018h
    db 0B4h, 0F8h, 0BBh, 0A3h, 052h, 0EFh, 0B0h, 014h
    db 0DDh, 010h, 05Bh, 0C8h, 09Fh, 041h, 0AAh, 03Fh
    db 066h, 07Fh, 063h, 05Fh, 0C0h, 048h, 07Fh, 028h

tlsks_expected_cats:
    db 019h, 063h, 082h, 07Bh, 028h, 008h, 075h, 075h
    db 08Ah, 034h, 055h, 04Ch, 007h, 013h, 029h, 0C5h
    db 016h, 076h, 0ECh, 0EDh, 053h, 059h, 022h, 0A9h
    db 003h, 0A5h, 037h, 095h, 073h, 0DAh, 03Fh, 09Bh

tlsks_expected_sats:
    db 0AAh, 054h, 00Ch, 042h, 029h, 03Ah, 012h, 003h
    db 0D7h, 023h, 0F8h, 0F4h, 0B8h, 0CFh, 0E8h, 084h
    db 0E4h, 0C3h, 03Ah, 072h, 073h, 0D8h, 043h, 02Ch
    db 0FCh, 0B8h, 03Ah, 0C6h, 0A7h, 0F1h, 095h, 047h

; ============================================================
; Buffers
; ============================================================

tlsks_fail_flag db 0

tlsks_early_secret times HKDF_HASH_SIZE db 0
tlsks_handshake_secret times HKDF_HASH_SIZE db 0
tlsks_chs times HKDF_HASH_SIZE db 0
tlsks_shs times HKDF_HASH_SIZE db 0
tlsks_cats times HKDF_HASH_SIZE db 0
tlsks_sats times HKDF_HASH_SIZE db 0
tlsks_input times 64 db 0


; ============================================================
; GhostTLS v0.5 - Runtime TLS 1.3 handshake secrets foundation
; Uses real tls_shared_secret from X25519 and transcript hash
; ClientHello || ServerHello.
; ============================================================

tls_derive_handshake_secrets:
    pushad

    cmp byte [tls_shared_secret_ready], 1
    jne .not_ready

    call tls_build_handshake_transcript_hash
    call tls_build_empty_hash

    ; early_secret = HKDF-Extract(zeros, zeros)
    mov esi, tls_runtime_zero_hash
    mov ecx, HKDF_HASH_SIZE
    mov edx, tls_runtime_zero_hash
    mov ebx, HKDF_HASH_SIZE
    mov edi, tls_runtime_early_secret
    call hmac_sha256_hash

    ; derived_secret = HKDF-Expand-Label(early_secret, "derived", SHA256(""))
    mov esi, tls_runtime_early_secret
    mov edi, tls_runtime_derived_secret
    mov edx, tls_label_derived
    mov ebx, tls_label_derived_len
    mov eax, tls_empty_hash
    mov ecx, HKDF_HASH_SIZE
    call tls_hkdf_expand_label_32

    ; handshake_secret = HKDF-Extract(derived_secret, shared_secret)
    mov esi, tls_runtime_derived_secret
    mov ecx, HKDF_HASH_SIZE
    mov edx, tls_shared_secret
    mov ebx, HKDF_HASH_SIZE
    mov edi, tls_handshake_secret
    call hmac_sha256_hash

    mov esi, tls_handshake_secret_msg
    call pm_print_string

    ; client_hs_traffic_secret = Expand-Label(handshake_secret, "c hs traffic", transcript_hash)
    mov esi, tls_handshake_secret
    mov edi, tls_client_hs_traffic_secret
    mov edx, tls_label_client_hs
    mov ebx, tls_label_client_hs_len
    mov eax, tls_transcript_hash
    mov ecx, HKDF_HASH_SIZE
    call tls_hkdf_expand_label_32

    mov esi, tls_client_hs_secret_msg
    call pm_print_string

    ; server_hs_traffic_secret = Expand-Label(handshake_secret, "s hs traffic", transcript_hash)
    mov esi, tls_handshake_secret
    mov edi, tls_server_hs_traffic_secret
    mov edx, tls_label_server_hs
    mov ebx, tls_label_server_hs_len
    mov eax, tls_transcript_hash
    mov ecx, HKDF_HASH_SIZE
    call tls_hkdf_expand_label_32

    mov esi, tls_server_hs_secret_msg
    call pm_print_string

    mov byte [tls_handshake_secrets_ready], 1
    jmp .done

.not_ready:
    mov esi, tls_handshake_not_ready_msg
    call pm_print_string

.done:
    popad
    ret


tls_build_handshake_transcript_hash:
    pushad

    call sha256_init

    ; Hash ClientHello as sent on wire.
    mov esi, tls_clienthello_payload
    mov ecx, tls_clienthello_len
    call sha256_update

    ; Hash first received TLS record, expected ServerHello.
    ; Total ServerHello record length = 5 + record_length.
    movzx eax, byte [rtl8139_tcp_payload_buffer + 3]
    shl eax, 8
    movzx edx, byte [rtl8139_tcp_payload_buffer + 4]
    or eax, edx
    add eax, 5

    ; Cap to stored payload length for safety.
    movzx ecx, word [rtl8139_tcp_last_payload_len]
    cmp eax, ecx
    jbe .len_ok
    mov eax, ecx

.len_ok:
    mov esi, rtl8139_tcp_payload_buffer
    mov ecx, eax
    call sha256_update

    mov edi, tls_transcript_hash
    call sha256_final

    mov esi, tls_transcript_hash_msg
    call pm_print_string

    popad
    ret


tls_build_empty_hash:
    pushad

    call sha256_init
    mov edi, tls_empty_hash
    call sha256_final

    popad
    ret


tls_hkdf_expand_label_32:
    ; ESI = secret / PRK ptr
    ; EDI = output ptr
    ; EDX = label ptr, without "tls13 "
    ; EBX = label len
    ; EAX = context ptr
    ; ECX = context len, normally 32
    ;
    ; HKDF-Expand-Label(secret, label, context, 32)
    ; HkdfLabel = uint16(32) || uint8(len("tls13 " + label))
    ;             || "tls13 " + label || uint8(context_len) || context
    ; OKM32 = HMAC(secret, HkdfLabel || 0x01)
    pushad

    mov [tls_hkdf_secret_ptr], esi
    mov [tls_hkdf_output_ptr], edi
    mov [tls_hkdf_label_ptr], edx
    mov [tls_hkdf_label_len], ebx
    mov [tls_hkdf_context_ptr], eax
    mov [tls_hkdf_context_len], ecx

    mov edi, tls_hkdf_label_buf

    ; uint16 length = 32
    mov byte [edi], 0
    inc edi
    mov byte [edi], 32
    inc edi

    ; label len = len("tls13 ") + label_len
    mov eax, [tls_hkdf_label_len]
    add eax, tls_label_prefix_len
    mov [edi], al
    inc edi

    ; copy "tls13 "
    mov esi, tls_label_prefix
    mov ecx, tls_label_prefix_len
.copy_prefix:
    cmp ecx, 0
    je .copy_label_start
    lodsb
    stosb
    dec ecx
    jmp .copy_prefix

.copy_label_start:
    mov esi, [tls_hkdf_label_ptr]
    mov ecx, [tls_hkdf_label_len]
.copy_label:
    cmp ecx, 0
    je .copy_context_len
    lodsb
    stosb
    dec ecx
    jmp .copy_label

.copy_context_len:
    mov eax, [tls_hkdf_context_len]
    mov [edi], al
    inc edi

    mov esi, [tls_hkdf_context_ptr]
    mov ecx, [tls_hkdf_context_len]
.copy_context:
    cmp ecx, 0
    je .append_counter
    lodsb
    stosb
    dec ecx
    jmp .copy_context

.append_counter:
    mov byte [edi], 1
    inc edi

    ; ebx = info_plus_counter length
    mov ebx, edi
    sub ebx, tls_hkdf_label_buf

    mov esi, [tls_hkdf_secret_ptr]
    mov ecx, HKDF_HASH_SIZE
    mov edx, tls_hkdf_label_buf
    mov edi, [tls_hkdf_output_ptr]
    call hmac_sha256_hash

    popad
    ret


tls_runtime_zero_hash times HKDF_HASH_SIZE db 0
tls_empty_hash times HKDF_HASH_SIZE db 0
tls_transcript_hash times HKDF_HASH_SIZE db 0

tls_runtime_early_secret times HKDF_HASH_SIZE db 0
tls_runtime_derived_secret times HKDF_HASH_SIZE db 0
tls_handshake_secret times HKDF_HASH_SIZE db 0
tls_client_hs_traffic_secret times HKDF_HASH_SIZE db 0
tls_server_hs_traffic_secret times HKDF_HASH_SIZE db 0

tls_handshake_secrets_ready db 0

tls_label_prefix db 'tls13 '
tls_label_prefix_len equ $ - tls_label_prefix

tls_label_derived db 'derived'
tls_label_derived_len equ $ - tls_label_derived

tls_label_client_hs db 'c hs traffic'
tls_label_client_hs_len equ $ - tls_label_client_hs

tls_label_server_hs db 's hs traffic'
tls_label_server_hs_len equ $ - tls_label_server_hs

tls_hkdf_label_buf times 128 db 0
tls_hkdf_secret_ptr dd 0
tls_hkdf_output_ptr dd 0
tls_hkdf_label_ptr dd 0
tls_hkdf_label_len dd 0
tls_hkdf_context_ptr dd 0
tls_hkdf_context_len dd 0

tls_transcript_hash_msg db 'transcript_hash built',13,0
tls_handshake_secret_msg db 'handshake_secret derived',13,0
tls_client_hs_secret_msg db 'client_hs_traffic_secret derived',13,0
tls_server_hs_secret_msg db 'server_hs_traffic_secret derived',13,0
tls_handshake_not_ready_msg db 'handshake secrets: shared_secret not ready',13,0

