; ============================================================
; GhostCrypto - RNG / Entropy
; v0.3
; ============================================================

rng_state dd 0x12345678
rng_counter dd 0

rng_seed:
    ; seed minimale iniziale
    ; per ora: mescola tick/contatore se disponibili
    push eax

    mov eax, [rng_state]
    xor eax, [pm_system_ticks]
    xor eax, [rng_counter]
    add eax, 0x9E3779B9
    mov [rng_state], eax

    pop eax
    ret

rng_next32:
    ; xorshift32 minimale
    push edx

    mov eax, [rng_state]
    test eax, eax
    jnz .ok
    mov eax, 0x12345678

.ok:
    mov edx, eax
    shl edx, 13
    xor eax, edx

    mov edx, eax
    shr edx, 17
    xor eax, edx

    mov edx, eax
    shl edx, 5
    xor eax, edx

    mov [rng_state], eax
    inc dword [rng_counter]

    pop edx
    ret

rngtest:
    call rng_seed

    mov esi, rng_test_msg
    call pm_print_string

    call rng_next32
    call pm_print_hex32
    call pm_newline

    call rng_next32
    call pm_print_hex32
    call pm_newline

    call rng_next32
    call pm_print_hex32
    call pm_newline

    ret

rng_test_msg db 'GhostCrypto RNG test',13,0